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

      number_cores = 1;
      processor_job core_map[number_cores];
      processor_job_map = core_map;

      channel_fifo_instruction pipes[number_cores];
      sc_fifo<short> result_pipes[number_cores];
      instruction_pipelines = pipes;
      result_pipelines = result_pipes;

      /*
      for(int i=0; i<number_cores; i++){
         printf("generating core%d \n", i);
         char name[5];
         sprintf(name, "core%d", i);
         processor* p = new processor(name);
         p->clk(clk);
         p->input(instruction_pipelines[i]);
         p->output(result_pipes[i]);
      }
      */

      //SC_THREAD(do_input);
      //SC_THREAD(do_output);

      //SC_METHOD(debug_clk)
      //      sensitive << clk.pos();

      SC_THREAD(test_stim);
   }

   void test_stim(){
      matrix m1;
      m1.initialize(2,2);
      bool h = true;

      wait(200,SC_NS);

      while(1){
         if(input->hasItems()){
            m1 = input->getItem();
            printf("multi: ");
            printf("%hd, %hd,", m1.get(0,0), m1.get(1,0));
            printf(" %hd, %hd\n", m1.get(0,1),m1.get(1,1));

            while(h){
               if(output->num_free()){
                  output->putItem(m1);
                  h = false;
               }
               else{
                  wait(20,SC_NS);
               }
            }
            h = true;

         }
         else{
            wait(200,SC_NS);
         }
      }
   }

   void do_input(){
      printf("mmult do_input\n");

      while(input->hasItems()){
         matrix m1 = input->getItem();
         matrix m2 = input->getItem();

         matrix_multiplication_job mjob(0, m1, m2);
         currently_processed_matrices.push_back(mjob);

         while(mjob.hasJobs()){
            processor_job pjob = mjob.getJob();

            bool found_empty = false;

            while(!found_empty){
               for(int i=0; i<number_cores; i++){
                  if(!instruction_pipelines[i].hasItems()){
                     found_empty = true;
                     //pjob merken für result zuordnung
                     processor_job_map[i] = pjob;

                     processor_instruction *instr = pjob.getInstructions();
                     printf("instruction %d, %d", instr[0].instruction, instr[0].data);
                     printf("instruction %d, %d", instr[1].instruction, instr[1].data);
                     instruction_pipelines[i].putItem(instr[0]);
                     instruction_pipelines[i].putItem(instr[1]);
                     break;
                  }
               }
            }
         }
      }
   }

   void do_output(){
      printf("mmult do_output\n");
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

   void debug_clk(){
      printf("clk event!\n");
   }
};

#endif
