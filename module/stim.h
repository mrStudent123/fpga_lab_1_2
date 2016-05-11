#ifndef stim_
#define stim_


#include "systemc.h"
#include <math.h>
#include "../src/matrix.h"

//#define number_testmatrix 100
//#define number_outputmatrix number_testmatrix/2

matrix input_array(2,2),[4];
matrix output_array[50](2,2);
int a[5];

SC_MODULE(stim) {

   SC_CTOR(stim) {
      //for(int i = 0; i < 100; i++){
         //input_array[i]
      //}

      SC_THREAD(go);

   }

   void go(){
      matrix* mtrix = new matrix(2,2);
      sc_fifo< processor_instruction > channel(16);

      wait(2,SC_NS);

      //mtrix->fillRandom(10);

   }

};

#endif



