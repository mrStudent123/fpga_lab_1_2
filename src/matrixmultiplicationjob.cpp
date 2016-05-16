/*
 * matrixmultiplicationjob.cpp
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#include "matrixmultiplicationjob.h"
#include "processorjob.h"
#include <stdio.h>

matrix_multiplication_job::matrix_multiplication_job(unsigned id, matrix matrix1, matrix matrix2) {

   _id = id;
   m1 = matrix1;
   m2 = matrix2;

   printf("new matrix job\n");
   m1.debug_print();
   m2.debug_print();

   result.initialize(m1.h, m2.w);
   num_received_add_results = 0;
   MAX_NUM_RECEIVED_ADD_RESULTS = result.w*result.h*(m1.w-1);

   for(int result_x=0; result_x < result.w; result_x++)
      {
         for(int result_y=0; result_y < result.h; result_y++)
         {
            for(int i=0; i<m1.w && i < m2.h; i++){
               processor_job job;
               job.calculation_id = _id;
               job.matrix_field = result_y * result.w + result_x;
               job.data1 = m1.get(result_y,i);
               job.data2 = m2.get(i, result_x);
               job.type = JOB_TYPE_MUL;
               jobList.push_back(job);
            }
         }
      }
}

matrix_multiplication_job::~matrix_multiplication_job() {
   // TODO Auto-generated destructor stub
}

matrix matrix_multiplication_job::getResult(){
   return result;
}

bool matrix_multiplication_job::hasJobs(){
   return jobList.size()>0;
}

processor_job matrix_multiplication_job::getJob(){
   processor_job job =  jobList.front();
   jobList.pop_front();
   return job;
}

bool matrix_multiplication_job::putJobResult(processor_job pjob, short value) {

   if(pjob.type == JOB_TYPE_ADD){
      num_received_add_results++;

      if(num_received_add_results >= MAX_NUM_RECEIVED_ADD_RESULTS ||
            (value == 0 && num_received_add_results >= (MAX_NUM_RECEIVED_ADD_RESULTS -1))){
         result.data[pjob.matrix_field] = value;
         return true;
      }
   }

   if(value == 0){
      return false;
   }

   // for both mul and add jobs
   if(result.data[pjob.matrix_field] == 0){
      result.data[pjob.matrix_field] = value;
   }
   else {
      processor_job addjob;
      addjob.calculation_id = _id;
      addjob.matrix_field = pjob.matrix_field;
      addjob.data2 = value;
      addjob.data1 = result.data[pjob.matrix_field];
      result.data[pjob.matrix_field] = 0;
      jobList.push_back(addjob);
   }

   return false;
}
