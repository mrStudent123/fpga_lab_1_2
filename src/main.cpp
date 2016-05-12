#include <stdio.h>
#include "systemc.h"

//#include "channel_processor_input_fifo.h"
#include "processor.h"
#include "stim.h"
#include "processor_instruction.h"

int sc_main(int argc, char* argv[]){

   sc_fifo< processor_instruction > inputFifo(16);

   //channel_processor_input_fifo input_channel;

   sc_clock clk ("clk", 2, SC_US);   // a clock with a period of 2 µ-sec

   processor processor_inst ("processor_inst");
   processor_inst.clk(clk);
   processor_inst.input(inputFifo);

   printf("start program");

   sc_trace_file *tf;          // Signal tracing
      tf=sc_create_vcd_trace_file("multicore");  // create new trace file
      tf->set_time_unit(1,SC_US);        // set time resolution to 0.5 �-sec
      //sc_trace(tf, amplitude_c, "Amplitude_c");


   sc_start(2000, SC_MS);

   sc_close_vcd_trace_file(tf);   // close trace file

   return 0;
}
