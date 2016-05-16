#ifndef stim_
#define stim_

#include "systemc.h"
#include <math.h>
#include "../src/matrix.h"
#include "channel_fifo_matrix.h"

#define number_testmatrix 100
#define number_outputmatrix number_testmatrix/2



SC_MODULE(stim) {

public:
   sc_port< if_fifo_matrix_in > data_out;
   sc_port< if_fifo_matrix_out > data_in;

   matrix input_array[number_testmatrix];
   matrix output_array[number_outputmatrix];
   matrix input_array_multiplied[number_outputmatrix];

private:


public:
   SC_CTOR(stim) {
      printf("stim constructor\n");

      matrix mul;
      mul.initialize(2,2);

      for(int i = 0; i < number_testmatrix; i++){
         input_array[i].initialize(2,2);
         input_array[i].initializeRandom_twoxtwo(10);
      }

      for(int i = 0; i < number_outputmatrix; i++){
         output_array[i].initialize(2,2);
         input_array_multiplied[i].initialize(2,2);
      }

      for(int i = 0; i < number_outputmatrix; i++){
         mul.initialize_value_twoxtwo(input_array[i*2]);
         mul.multiply_twoxtwo(input_array[i*2+1]);
         input_array_multiplied[i].initialize_value(mul);
      }

      // print: testvalues
      /*
      printf("---test values---\n");
      for(int i = 0; i < number_testmatrix; i++){
         printf("%d:",i);
         printf("%hd, %hd,", input_array[i].get(0,0), input_array[i].get(1,0));
         printf(" %hd, %hd\n", input_array[i].get(0,1),input_array[i].get(1,1));
      }*/

      // print: test values multiplied
      /*
      printf("---test values multiplied---\n");
      for(int i = 0; i < number_outputmatrix; i++){
         printf("%d:",i);
         printf("%hd, %hd,", input_array_multiplied[i].get(0,0), input_array_multiplied[i].get(1,0));
         printf(" %hd, %hd\n", input_array_multiplied[i].get(0,1),input_array_multiplied[i].get(1,1));
      }*/


      SC_THREAD(write);

      SC_THREAD(read);

      //SC_THREAD(compare);
   }

   void write(){

      printf("stim thread write\n");

      wait(2,SC_NS);

      int i = 0;

      while(i < number_testmatrix){
         if(data_out->num_free()){
            data_out->putItem(input_array[i]);
            printf("stim writing matrix %d\n", i);
            i++;
         }
         else{
            //printf("stim waiting, fifo full");
            wait(20,SC_NS);
         }
      }
   }

   void read(){
      matrix a;
      a.initialize(2,2);

      while(1){

         if(data_in->hasItems()){
            a = data_in->getItem();
            printf("stim: ");
            printf("%hd, %hd,", a.get(0,0), a.get(1,0));
            printf(" %hd, %hd\n", a.get(0,1),a.get(1,1));
         }
         else{
            wait(200,SC_NS);
         }

      }
   }

   void compare(){
      matrix a;

      printf("stim thread compare\n");

      while(1){

         for(int j = 0; j < number_outputmatrix;){
            if(data_in->hasItems()){
               a.initialize_value(input_array[j*2]);
               a.multiply(input_array[j*2+1]);
               output_array[j] = data_in->getItem();
               j++;

               //assert( a.equals(output_array[j]));
               //SC_REPORT_INFO("error ","error");
            }
            else{
               wait(2, SC_US);
            }

         }
      }
   }
};

//cout << "Multiplication of:" << j*2 << " OK, time: "<< sc_time_stamp() << endl;

#endif



