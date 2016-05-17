#ifndef stim_
#define stim_

#include "systemc.h"
#include <math.h>
#include "../src/matrix.h"
#include "channel_fifo_matrix.h"

#define number_testmatrix 4
#define number_outputmatrix number_testmatrix/2



SC_MODULE(stim) {

public:
   sc_port< if_fifo_matrix_in > data_out;
   sc_port< if_fifo_matrix_out > data_in;

   matrix input_array[number_testmatrix];
   matrix output_array[number_outputmatrix];
   matrix input_array_multiplied[number_outputmatrix];

private:
   unsigned short read_count;

public:
   SC_CTOR(stim) {
      //printf("stim constructor\n");

      read_count = 0;

      matrix mul;
      mul.initialize(2,2);

      for(int i = 0; i < number_testmatrix; i++){
         input_array[i].initialize(2,2);
         input_array[i].initializeRandom_twoxtwo(10);
      }

      for(int i = 0; i < number_outputmatrix; i++){
         output_array[i].initialize(2,2);
         input_array_multiplied[i].initialize(2,2);
      }

      for(int i = 0; i < number_outputmatrix; i++){
         mul.initialize_value_twoxtwo(input_array[i*2]);
         mul.multiply_twoxtwo(input_array[i*2+1]);
         input_array_multiplied[i].initialize_value(mul);
      }

      SC_THREAD(write);
      SC_THREAD(read);
   }

   void write(){

      //printf("stim thread write\n");

      wait(2,SC_NS);

      int i = 0;

      while(i < number_testmatrix){
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
      matrix a;
      a.initialize(2,2);

      while(read_count < (number_outputmatrix)){

         if(data_in->hasItems()){
            a = data_in->getItem();
            printf("stim received result: ");
            a.debug_print();

            if(a.equals(input_array_multiplied[read_count])){
               printf("result is OK! :)\n");
            }
            else {
               printf("result is wrong, should be ");
               input_array_multiplied[read_count].debug_print();
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



