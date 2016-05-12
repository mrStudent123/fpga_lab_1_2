#include <stdio.h>
#include "systemc.h"

#include "processor_instruction.h"
#include "processor.h"
#include "stim.h"

#include "processor_instruction.h"

#include "channel_fifo_matrix.h"
#include "matrix_multiplicator.h"


int sc_main(int argc, char* argv[]){

   channel_fifo_matrix inputFifo;
   channel_fifo_matrix outputFifo;

   sc_clock clk ("clk", 2, SC_US);   // a clock with a period of 2 µ-sec

   stim stim_inst("stim");
   stim_inst.data_in(outputFifo);
   stim_inst.data_out(inputFifo);

   matrix_multiplicator multiplicator ("multipl");
   multiplicator.clk(clk);
   multiplicator.input(inputFifo);
   multiplicator.output(outputFifo);

   printf("start program");

   sc_trace_file *tf;          // Signal tracing
   tf=sc_create_vcd_trace_file("multicore");  // create new trace file
   tf->set_time_unit(1,SC_US);        // set time resolution to 0.5 �-sec
   //sc_trace(tf, amplitude_c, "Amplitude_c");


   sc_start(2000, SC_MS);

   sc_close_vcd_trace_file(tf);   // close trace file

   return 0;
}
