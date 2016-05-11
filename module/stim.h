#ifndef stim_
#define stim_


#include "systemc.h"
#include <math.h>
#include "matrix.h"


SC_MODULE(stim) {
   matrix mtrix;

   SC_CTOR(stim) {

      SC_THREAD(go);

   }

   void go(){
      wait(2,SC_US);



   }

};

#endif



