#include <stdio.h>
#include "systemc.h"

#include "processor_instruction.h"
#include "processor.h"
#include "stim.h"

#include "processor_instruction.h"

#include "channel_fifo_matrix.h"
#include "matrix_multiplicator.h"

#include "stim_matrix_channel_test.h"


int sc_main(int argc, char* argv[]){

   channel_fifo_matrix inputFifo;
   channel_fifo_matrix outputFifo;

   sc_clock clk ("clk", 1, SC_NS);   // a clock with a period of 2 µ-sec

   /* MATRIX CHANNEL TEST
   channel_fifo_matrix testchannel;
   stim_channel_matrix_test stim_test("testinger");
   stim_test.input(testchannel);
   stim_test.output(testchannel);
   stim_test.clk(clk); */

   /* INSTRUCTION CHANNEL TEST
   channel_fifo_instruction testchannel;
   stim_channel_test stim_test("testinger");
   stim_test.input(testchannel);
   stim_test.output(testchannel);
   stim_test.clk(clk);*/

   stim stim_inst("stim");
   stim_inst.data_in(outputFifo);
   stim_inst.data_out(inputFifo);

   matrix_multiplicator multiplicator ("multipl");
   multiplicator.clk(clk);
   multiplicator.input(inputFifo);
   multiplicator.output(outputFifo);

   sc_trace_file *tf;          // Signal tracing
   tf=sc_create_vcd_trace_file("main");  // create new trace file
   tf->set_time_unit(0.01,SC_NS);        // set time resolution to 0.5 �-sec
   sc_trace(tf, inputFifo.count , "channel_stim_output");
   sc_trace(tf, outputFifo.count , "channel_stim_input");


   printf("\nstart program\n\n");
   sc_start(1, SC_MS);

   sc_close_vcd_trace_file(tf);   // close trace file

   return 0;
}
