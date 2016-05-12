#ifndef __MATRIX_MUL_MODULE_H___
#define __MATRIX_MUL_MODULE_H___

#include "systemc.h"
#include "../src/matrix.h"
#include "channel_fifo_matrix.h"
#include "channel_fifo_instruction.h"
#include "../src/matrixmultiplicationjob.h"

SC_MODULE(matrix_multiplicator){

   sc_port < if_fifo_matrix_out > input;
   sc_port < if_fifo_matrix_in > output;
   sc_in <bool> clk;

   typedef std::list<matrix_multiplication_job> MatrixList;
   MatrixList currently_processed_matrices;

   unsigned short number_cores;
   processor_job *processor_job_map;

   channel_fifo_instruction *instruction_pipelines;
   sc_fifo<short> *result_pipelines;

   SC_CTOR(matrix_multiplicator){

      printf("mmult constructor\n");

      number_cores = 12;
      processor_job core_map[number_cores];
      processor_job_map = core_map;

      channel_fifo_instruction pipes[number_cores];
      sc_fifo<short> result_pipes[number_cores];
      instruction_pipelines = pipes;
      result_pipelines = result_pipes;

      for(int i=0; i<number_cores; i++){
         char name[5];
         sprintf(name, "core%d", i);
         processor* p = new processor(name);
         p->clk(clk);
         p->input(instruction_pipelines[i]);
         p->output(result_pipes[i]);
      }

      SC_THREAD(do_input);
      SC_THREAD(do_output);
   }

   void do_input(){
      while(input->hasItems()){
         matrix m1 = input->getItem();
         matrix m2 = input->getItem();

         matrix_multiplication_job mjob(0, m1, m2);
         currently_processed_matrices.push_back(mjob);

         while(mjob.hasJobs()){
            processor_job pjob = mjob.getJob();

            processor_instruction *instr = pjob.getInstructions();

            bool found_empty = false;
            int core_to_feed = 0;

            while(!found_empty){
               for(int i=0; i<number_cores; i++){
                  if(!instruction_pipelines[i].hasItems()){
                     core_to_feed = i;
                     found_empty = true;
                     //pjob merken für result zuordnung
                     processor_job_map[i] = pjob;
                     break;
                  }
               }
            }

            if(instruction_pipelines[core_to_feed].num_free() >= 2){
               instruction_pipelines[core_to_feed].putItem(instr[0]);
               instruction_pipelines[core_to_feed].putItem(instr[1]);
            }
         }
      }
   }

   void do_output(){
      while(1){

         //check for processor outputs
         for(int i=0; i<number_cores; i++){
            if(result_pipelines[i].num_available() > 0){

               for(MatrixList::iterator it = currently_processed_matrices.begin();
                     it != currently_processed_matrices.end();
                     it++)
               {
                  if((*it)._id == processor_job_map[i].calculation_id){
                     if((*it).putJobResult(processor_job_map[i], result_pipelines[i].read())){
                        //fertige matrix rausschreiben
                        output->putItem((*it).getResult());
                        currently_processed_matrices.erase(it);
                     }
                     break;
                  }
               }
            }
         }
      }
   }
};

#endif
