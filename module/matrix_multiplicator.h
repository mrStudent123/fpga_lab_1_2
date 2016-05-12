#ifndef __MATRIX_MUL_MODULE_H___
#define __MATRIX_MUL_MODULE_H___

#include "systemc.h"
#include "../src/matrix.h"
#include "channel_fifo_matrix.h"
#include "channel_fifo_instruction.h"
#include "../src/matrixmultiplicationjob.h"

SC_MODULE(matrix_multiplicator){

   sc_port < if_fifo_matrix_out > input;
   sc_in <bool> clk;

   int counter;
   unsigned short number_cores;

   int core_to_feed;

   channel_fifo_instruction *instruction_pipelines;

   SC_CTOR(matrix_multiplicator){

      counter = 0;
      core_to_feed = 0;

      number_cores = 12;

      channel_fifo_instruction pipes[number_cores];
      instruction_pipelines = pipes;

      for(int i=0; i<number_cores; i++){
         char name[5];
         sprintf(name, "core%d", i);
         processor* p = new processor(name);
         p->clk(clk);
         p->input(instruction_pipelines[i]);
      }

      SC_THREAD(do_stuff);
   }

   void do_stuff(){
      while(input->hasItems()){
         matrix m1 = input->getItem();
         matrix m2 = input->getItem();

         matrix_multiplication_job job(0, m1, m2);

         bool full = false;
         while(job.hasJobs() && !full){
            processor_job pjob = job.getJob();

            processor_instruction *instr = pjob.getInstructions();

            if(instruction_pipelines[core_to_feed].num_free() >= 2){
               instruction_pipelines[core_to_feed].putItem(instr[0]);
               instruction_pipelines[core_to_feed].putItem(instr[1]);
            }
            else {
               full = true;
            }
         }

         counter++;
      }
   }
};

#endif
