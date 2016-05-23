#include <stdio.h>
#include "systemc.h"
#include "string.h"

#include "processor_instruction.h"
#include "processor.h"
#include "stim.h"

#include "processor_instruction.h"

#include "channel_fifo_matrix.h"
#include "matrix_multiplicator.h"

#include "stim_matrix_channel_test.h"

void write_file();


int sc_main(int argc, char* argv[]){

   channel_fifo_matrix inputFifo;
   channel_fifo_matrix outputFifo;

   sc_clock clk ("clk", 1, SC_NS);   // a clock with a period of 2 µ-sec

   char trace_comment[200];
   sprintf(trace_comment, "\n core number: %d\n number matrix %d\n multiplication of %dx%d with %dx%d\n input wait %d\n output wait %d\n",
         NUMBER_CORES,
         INPUT_SIZE,
         MATRIX_SIZE_Y_EVEN,
         MATRIX_SIZE_X_EVEN,
         MATRIX_SIZE_Y_ODD,
         MATRIX_SIZE_X_ODD,
         WAIT_AFTER_CHANNEL_OUT_FULL,
         WAIT_AFTER_CHANNEL_IN_FULL);


   tf=sc_create_vcd_trace_file("main");  // create new trace file
   tf->write_comment(trace_comment);


   tf->set_time_unit(0.1,SC_NS);        // set time resolution to 0.5 �-sec
   sc_trace(tf, inputFifo.count , "channel_stim_output");
   sc_trace(tf, outputFifo.count , "channel_stim_input");

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


   printf("\nstart program\n\n");
   sc_start(5, SC_MS);

   sc_close_vcd_trace_file(tf);   // close trace file

   write_file();

   cout << "write in file: done" << endl;

   return 0;
}

void write_file(){
   FILE * logfile;

   logfile = fopen ("logfile.txt", "a");
   fprintf(logfile, "core_number: %d   number_matrix: %d   %dx%d with %dx%d   input_wait: %d   output_wait: %d   time_stamp: %s\n",
         NUMBER_CORES,
         INPUT_SIZE,
         MATRIX_SIZE_Y_EVEN,
         MATRIX_SIZE_X_EVEN,
         MATRIX_SIZE_Y_ODD,
         MATRIX_SIZE_X_ODD,
         WAIT_AFTER_CHANNEL_OUT_FULL,
         WAIT_AFTER_CHANNEL_IN_FULL,
         global_time_stamp);


   fclose(logfile);

}
