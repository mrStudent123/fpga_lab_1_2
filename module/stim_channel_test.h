#include "systemc.h"
#include "channel_fifo_instruction.h"
#include "processor_instruction.h"

SC_MODULE(stim_channel_test) {

   sc_port< if_fifo_instruction_out > input;
   sc_port< if_fifo_instruction_in > output;
   sc_in<bool> clk;


   SC_CTOR(stim_channel_test) {
      SC_METHOD(do_test)
               sensitive << clk.pos();
   }

   void do_test(){
      if(input->hasItems()){
         processor_instruction inst = input->getItem();
         printf("received instruction: %d, %d \n", inst.instruction, inst.data);
      }
      else {
         printf("writing instruction\n");
         processor_instruction inst;
         inst.data = 1;
         inst.instruction = 2;
         output->putItem(inst);
      }
   }
};
