#ifndef stim_
#define stim_


#include "systemc.h"
#include <math.h>
#include "../src/matrix.h"

#define number_testmatrix 100
#define number_outputmatrix number_testmatrix/2

matrix input_array[number_testmatrix];
matrix output_array[number_outputmatrix];

SC_MODULE(stim) {

   sc_port< sc_fifo_out_if<matrix> > data_out;
   sc_port< sc_fifo_in_if<matrix> > data_in;
   int j;

   SC_CTOR(stim) {
      for(int i = 0; i < number_testmatrix; i++){
         input_array[i].initializeRandom(2,2,10);
      }

      SC_THREAD(write);

      SC_METHOD(compare);
      sensitive << data_in->data_written_event();

   }

   void write(){

      wait(2,SC_NS);

      for(int i = 0; i < number_testmatrix; i++){
         if(data_out->num_free()){
            data_out->write(input_array[i]);
         }
         else{
            wait(2,SC_NS);
         }
      }
   }

   void compare(){

      assert();

      for(j = 0; j < number_outputmatrix; j++){
            output_array[i] = channel_in.read();
      }
   }

};

#endif



