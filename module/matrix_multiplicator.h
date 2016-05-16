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
   channel_fifo_short *result_pipelines;

   SC_CTOR(matrix_multiplicator){

      printf("mmult constructor\n");

      number_cores = 10;

      processor_job_map = new processor_job[number_cores];

      instruction_pipelines = new channel_fifo_instruction[number_cores];;
      result_pipelines = new channel_fifo_short[number_cores];


      for(int i=0; i<number_cores; i++){
         printf("generating core%d \n", i);
         char name[5];
         sprintf(name, "core%d", i);
         processor* p = new processor(name);
         p->clk(clk);
         p->input(instruction_pipelines[i]);
         p->output(result_pipelines[i]);
      }

      SC_METHOD(do_input);
           sensitive << clk.neg();
      SC_METHOD(do_output);
           sensitive << clk.neg();

      //SC_METHOD(debug_clk)
      //     sensitive << clk.pos();

      //SC_THREAD(test_stim);
   }
/*
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

            //output->putItem(m1);

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
   }*/

   void do_input(){

      //printf("checking for idle core\n");

      for(int i=0; i<number_cores; i++){
         if(!instruction_pipelines[i].hasItems()){

            bool found = false;

            //matrizen durchgehn und schaun obs noch jobs gibt
            for(MatrixList::iterator it = currently_processed_matrices.begin();
               it != currently_processed_matrices.end();
               it++)
            {
               if((*it).hasJobs()){
                  found = true;
                  process(*it, i);
                  break;
               }
            }

            // no jobs found from currently processed matrices
            if(!found && (input->num_available() >= 2))
            {
               matrix m1 = input->getItem();
               matrix m2 = input->getItem();

               printf("new matrix input\n");

               matrix_multiplication_job mjob(0, m1, m2);
               currently_processed_matrices.push_back(mjob);
               process(mjob, i);
            }
         }
      }
   }

   void process(matrix_multiplication_job mjob, int i){
      processor_job pjob = mjob.getJob();

      //pjob merken für result zuordnung
      processor_job_map[i] = pjob;

      processor_instruction *instr = pjob.getInstructions();
      //printf("instruction %d, %d \n", instr[0].instruction, instr[0].data);
      //printf("instruction %d, %d \n", instr[1].instruction, instr[1].data);
      instruction_pipelines[i].putItem(instr[0]);
      instruction_pipelines[i].putItem(instr[1]);
   }

   void do_output(){
      //printf("mmult do_output for %d cores\n", number_cores);

      //check for processor outputs
      for(int i=0; i<number_cores; i++){
         if(result_pipelines[i].hasItems()){
            //printf("results available in %d\n", i);
            for(MatrixList::iterator it = currently_processed_matrices.begin();
                  it != currently_processed_matrices.end();
                  it++)
            {
               if((*it)._id == processor_job_map[i].calculation_id){
                  if((*it).putJobResult(processor_job_map[i], result_pipelines[i].getItem())){
                     //fertige matrix rausschreiben
                     printf("finished calculation\n");
                     output->putItem((*it).getResult());
                     currently_processed_matrices.erase(it);
                  }
                  break;
               }
            }
         }
         else {
            //printf("no results to process\n");
         }
      }
   }

   void debug_clk(){
      printf("clk event!\n");
   }
};

#endif
