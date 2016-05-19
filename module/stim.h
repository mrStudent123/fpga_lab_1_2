#ifndef stim_
#define stim_

#include "systemc.h"
#include <math.h>
#include "../src/matrix.h"
#include "channel_fifo_matrix.h"

#define INPUT_SIZE 10
#define OUTPUT_SIZE INPUT_SIZE/2

#define MATRIX_SIZE_X_EVEN 3
#define MATRIX_SIZE_Y_EVEN 3

#define MATRIX_SIZE_X_ODD 3
#define MATRIX_SIZE_Y_ODD 3

#define RANDOM_FILL_MAX 10

#define WAIT_AFTER_CHANNEL_OUT_FULL 2
#define WAIT_AFTER_CHANNEL_IN_FULL 2

sc_trace_file *tf;          // Signal tracing

char global_time_stamp[35];

SC_MODULE(stim) {

public:
   sc_port< if_fifo_matrix_in > data_out;
   sc_port< if_fifo_matrix_out > data_in;

   matrix input_array[INPUT_SIZE];

private:
   unsigned short read_count;
   unsigned short count_result_true;
   unsigned short count_result_false;

public:
   SC_CTOR(stim) {
      read_count = 0;
      count_result_true = 0;
      count_result_false = 0;

      if(MATRIX_SIZE_X_EVEN != MATRIX_SIZE_Y_ODD){
         printf("matrix sizes not applicable for multiplication, exiting\n");
         abort();
      }

      for(int i = 0; i < INPUT_SIZE; i++){
         if(i%2 != 0){
            input_array[i].initializeRandom(MATRIX_SIZE_X_EVEN,MATRIX_SIZE_Y_EVEN,RANDOM_FILL_MAX);
         }
         else {
            input_array[i].initializeRandom(MATRIX_SIZE_X_ODD,MATRIX_SIZE_Y_ODD,RANDOM_FILL_MAX);
         }
      }

      SC_THREAD(write);
      SC_THREAD(read);
   }

   void write(){

      wait(2,SC_NS);

      int i = 0;

      while(i < INPUT_SIZE){
         if(data_out->num_free()){
            data_out->putItem(input_array[i]);
            //printf("stim writing matrix %d\n", i);
            i++;
         }
         else{
            //printf("stim waiting, fifo full");
            wait(WAIT_AFTER_CHANNEL_OUT_FULL,SC_NS);
         }
      }
   }

   void read(){
      while(read_count < (OUTPUT_SIZE)){

         if(data_in->hasItems()){
            matrix a = data_in->getItem();
            printf("stim received result %d of %d:\n", read_count+1, OUTPUT_SIZE);
            a.debug_print();

            matrix compare = input_array[read_count*2].multiply(input_array[read_count*2+1]);

            if(a.equals(compare)){
               printf("result is OK! :)\n\n");
               count_result_true++;
            }
            else {
               printf("result is wrong, should be\n");
               compare.debug_print();
               printf("\n");
               count_result_false++;
            }

            read_count++;
         }
         else{
            wait(WAIT_AFTER_CHANNEL_IN_FULL,SC_NS);
         }

      }

      printf("\nfinished all calculations\n");
      printf("number values: %hd\n", INPUT_SIZE);
      printf("%hdx%hd %hdx%hd\n", MATRIX_SIZE_Y_EVEN, MATRIX_SIZE_X_EVEN, MATRIX_SIZE_Y_ODD,MATRIX_SIZE_X_ODD);
      printf("number result true: %hd\n", count_result_true);
      printf("number result true: %hd\n", count_result_false);
      cout << "time: "<< sc_time_stamp() << endl;

      strcpy(global_time_stamp, sc_time_stamp().to_string().c_str());

      //abort();
   }
};

#endif



