#include "systemc.h"
#include "channel_fifo_matrix.h"
#include "../src/matrix.h"

SC_MODULE(stim_channel_matrix_test) {

   sc_port< if_fifo_matrix_out > input;
   sc_port< if_fifo_matrix_in > output;
   sc_in<bool> clk;


   SC_CTOR(stim_channel_matrix_test) {
      SC_METHOD(do_test)
               sensitive << clk.pos();
   }

   void do_test(){
      if(input->hasItems()){
         matrix m = input->getItem();
         printf("received matrix\n");
         m.debug_print();
      }
      else {
         matrix m;
         m.initializeRandom(2,2,5);
         printf("writing matrix\n");
         m.debug_print();
         output->putItem(m);
      }
   }
};
