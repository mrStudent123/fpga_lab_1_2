#ifndef __MATRIX_MUL_MODULE_H___
#define __MATRIX_MUL_MODULE_H___

#include "systemc.h"
#include "../src/matrix.h"
#include "channel_fifo_matrix.h"
#include "channel_fifo_instruction.h"
#include "../src/matrixmultiplicationjob.h"
#include "stim.h"

#define NUMBER_CORE 1

SC_MODULE(matrix_multiplicator){

   sc_port < if_fifo_matrix_out > input;
   sc_port < if_fifo_matrix_in > output;
   sc_in <bool> clk;

   unsigned short number_of_calculations;

   typedef std::list<matrix_multiplication_job> MatrixList;
   MatrixList *currently_processed_matrices;

   unsigned short number_cores;
   processor_job *processor_job_map;

   channel_fifo_instruction *instruction_pipelines;
   channel_fifo_short *result_pipelines;

   long matrix_counter;

   SC_CTOR(matrix_multiplicator){

      number_of_calculations = 0;

      number_cores = NUMBER_CORE;

      matrix_counter = 0;

      currently_processed_matrices = new MatrixList;
      processor_job_map = new processor_job[number_cores];

      instruction_pipelines = new channel_fifo_instruction[number_cores];;
      result_pipelines = new channel_fifo_short[number_cores];

      for(int i=0; i<number_cores; i++){
         printf("initializing core %d \n", i);
         char name[5];
         sprintf(name, "core%d", i);
         processor* p = new processor(name);
         p->clk(clk);
         p->input(instruction_pipelines[i]);
         p->output(result_pipelines[i]);

      }


      trace_multiplicator();

      SC_METHOD(do_input);
           sensitive << clk.neg();
      SC_METHOD(do_output);
           sensitive << clk.neg();


      //sc_close_vcd_trace_file(tf);   // close trace file

      //SC_THREAD(test_stim);
   }


   void do_input(){

      //printf("checking for idle core\n");

      for(int i=0; i<number_cores; i++){
         if(!instruction_pipelines[i].hasItems()){

            bool found = false;

            //matrizen durchgehn und schaun obs noch jobs gibt
            for(MatrixList::iterator it = (*currently_processed_matrices).begin();
               it != (*currently_processed_matrices).end();
               it++)
            {
               //printf("checking processed matrices\n");

               if((*it).hasJobs()){
                  //printf("mjob has pjobs\n");
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

               printf("new matrix input %ld %ld, currently processing %lu matrices in list\n",matrix_counter, matrix_counter + 1, (*currently_processed_matrices).size()+1);

               matrix_multiplication_job mjob(number_of_calculations++, m1, m2);
               (*currently_processed_matrices).push_back(mjob);
               process(mjob, i);

               matrix_counter = matrix_counter + 2;
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

            for(MatrixList::iterator it = (*currently_processed_matrices).begin();
                  it != (*currently_processed_matrices).end();
                  it++)
            {
               if((*it)._id == processor_job_map[i].calculation_id){
                  if((*it).putJobResult(processor_job_map[i], result_pipelines[i].getItem())){
                     //fertige matrix rausschreiben
                     //printf("finished calculation\n\n");
                     output->putItem((*it).getResult());
                     (*currently_processed_matrices).erase(it);
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

   void trace_multiplicator(){

      char string[15];

      for(int i = 0; i < NUMBER_CORE; i++){

         sprintf(string, "core%d", i);
         sc_trace(tf, instruction_pipelines[i].count , string);

         sprintf(string, "core%d_data", i);
         sc_trace(tf, instruction_pipelines[i].temp.data , string);

         sprintf(string, "core%d_instr", i);
         sc_trace(tf, instruction_pipelines[i].temp.instruction , string);

         sprintf(string, "core%d_result", i);
         sc_trace(tf, result_pipelines[i].temp_trace , string);
      }

   }

};



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

#endif
