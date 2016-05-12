#ifndef stim_
#define stim_

#include "systemc.h"
#include <math.h>
#include "../src/matrix.h"
#include "channel_fifo_matrix.h"

#define number_testmatrix 100
#define number_outputmatrix number_testmatrix/2

matrix input_array[number_testmatrix];
matrix output_array[number_outputmatrix];

SC_MODULE(stim) {

   sc_port< if_fifo_matrix_in > data_out;
   sc_port< if_fifo_matrix_out > data_in;
   int j;

   SC_CTOR(stim) {
      j = 0;

      for(int ini = 0; ini < number_testmatrix; ini++){
         input_array[ini].initializeRandom(2,2,10);
      }

      SC_THREAD(write);

      SC_THREAD(compare);
   }

   void write(){

      wait(2,SC_NS);

      for(int i = 0; i < number_testmatrix; i++){
         if(data_out->num_free()){
            data_out->putItem(input_array[i]);
         }
         else{
            wait(2,SC_NS);
         }
      }
   }

   void compare(){

      while(1){

         if(data_in->hasItems()){
            matrix a;
            //matrix b;

            for(j = 0; j < number_outputmatrix; j++){

               a.initialize_value(input_array[j*2]);
               a.multiply(input_array[j*2+1]);
               output_array[j] = data_in->getItem();

               assert( a.equals(output_array[j]));
               SC_REPORT_INFO("error ","error");

               cout << "Multiplication of:" << j*2 << " OK, time: "<< sc_time_stamp() << endl;
            }
         }

         wait(2, SC_NS);
      }
   }

};

#endif



