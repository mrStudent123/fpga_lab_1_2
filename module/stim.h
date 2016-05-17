#ifndef stim_
#define stim_

#include "systemc.h"
#include <math.h>
#include "../src/matrix.h"
#include "channel_fifo_matrix.h"

#define INPUT_SIZE 4
#define OUTPUT_SIZE INPUT_SIZE/2

#define MATRIX_SIZE_X 4
#define MATRIX_SIZE_Y 4
#define RANDOM_FILL_MAX 10


SC_MODULE(stim) {

public:
   sc_port< if_fifo_matrix_in > data_out;
   sc_port< if_fifo_matrix_out > data_in;

   matrix input_array[INPUT_SIZE];

private:
   unsigned short read_count;

public:
   SC_CTOR(stim) {
      read_count = 0;

      for(int i = 0; i < INPUT_SIZE; i++){
         input_array[i].initializeRandom(MATRIX_SIZE_X,MATRIX_SIZE_Y,RANDOM_FILL_MAX);
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
            wait(20,SC_NS);
         }
      }
   }

   void read(){
      while(read_count < (OUTPUT_SIZE)){

         if(data_in->hasItems()){
            matrix a = data_in->getItem();
            printf("stim received result: ");
            a.debug_print();

            matrix compare = input_array[read_count*2].multiply(input_array[read_count*2+1]);

            if(a.equals(compare)){
               printf("result is OK! :)\n\n");
            }
            else {
               printf("result is wrong, should be ");
               compare.debug_print();
               printf("\n");
            }

            read_count++;
         }
         else{
            wait(200,SC_NS);
         }

      }

      printf("\nfinished all calculations\n");
      cout << "time: "<< sc_time_stamp() << endl;

      abort();
   }
};

#endif



