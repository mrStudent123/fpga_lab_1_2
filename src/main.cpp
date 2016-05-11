#include <stdio.h>
#include "systemc.h"

//#include "channel_processor_input_fifo.h"
#include "processor.h"
#include "stim.h"

int sc_main(int argc, char* argv[]){

   sc_fifo< processor_instruction > inputFifo(16);

   //channel_processor_input_fifo input_channel;

   sc_clock clk ("clk", 2, SC_US);   // a clock with a period of 2 µ-sec

   processor processor_inst ("processor_inst");
   processor_inst.clk(clk);
   processor_inst.input(inputFifo);

   printf("start program");

   sc_start(2000, SC_MS);

   return 0;
}
