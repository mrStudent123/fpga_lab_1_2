#ifndef ___PROCESSOR_DEF___
#define ___PROCESSOR_DEF___

#include "systemc.h"
#include "../src/processor_instruction.h"
#include "channel_fifo_instruction.h"

const unsigned short INSTR_NOP = 0x00;
const unsigned short INSTR_SETA = 0x01;
const unsigned short INSTR_ADD = 0x02;
const unsigned short INSTR_MUL = 0x03;


SC_MODULE(processor) {

   short regA;

   sc_port< if_fifo_instruction_out > input;
   sc_in<bool> clk;
   sc_port < sc_fifo_out_if<short> > output;

   SC_CTOR(processor) {
      regA = 0;

      //SC_METHOD(do_stuff)
      //      sensitive << clk.pos();
   }

   void do_stuff(){
      printf("load\n");

      if(input->hasItems()){
         return;
      }

      processor_instruction data = input->getItem();

      switch(data.instruction)
      {
         case INSTR_ADD:
            regA += data.data;
            output->write(regA);
         break;

         case INSTR_MUL:
            regA *= data.data;
            output->write(regA);
         break;

         case INSTR_SETA:
            regA = data.data;
         break;

         case INSTR_NOP:
         default:
            break;
      }
   }
};


#endif
