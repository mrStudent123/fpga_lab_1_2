/*
 * matrixmultiplicationjob.cpp
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#include "matrixmultiplicationjob.h"
#include "processorjob.h"
#include <stdio.h>
#include <iostream>

matrix_multiplication_job::matrix_multiplication_job(unsigned id, matrix matrix1, matrix matrix2) {

   _id = id;
   m1 = matrix1;
   m2 = matrix2;

   jobList = new PJobList();

   printf("new matrix job\n");
   m1.debug_print();
   m2.debug_print();
   printf("\n");

   result.initialize(m1.h, m2.w);
   num_received_add_results = 0;
   MAX_NUM_RECEIVED_ADD_RESULTS = result.w*result.h*(m1.w-1);

   //printf("initial job list size %lu\n", (*jobList).size());

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
            (*jobList).push_back(job);
            job.debug_print();
            //printf("Adding job to job list\n");
         }
      }
   }

   //printf("new job list size %lu\n", (*jobList).size());
}

matrix_multiplication_job::~matrix_multiplication_job() {
   // TODO Auto-generated destructor stub
}

matrix matrix_multiplication_job::getResult(){
   return result;
}

bool matrix_multiplication_job::hasJobs(){
   return (*jobList).size()>0;
}

processor_job matrix_multiplication_job::getJob(){
   //printf("reading from job list, size %lu\n", (*jobList).size());
   processor_job job =  (*jobList).front();
   (*jobList).pop_front();
   //printf("%d size after reading %lu\n", _id, (*jobList).size());
   return job;
}

bool matrix_multiplication_job::putJobResult(processor_job pjob, short value) {

   if(pjob.type == JOB_TYPE_ADD){
      //printf("%d received add job result\n", _id);
      num_received_add_results++;

      if(num_received_add_results >= MAX_NUM_RECEIVED_ADD_RESULTS ||
            (value == 0 && num_received_add_results >= (MAX_NUM_RECEIVED_ADD_RESULTS -1))){
         result.data[pjob.matrix_field] = value;

         printf("%d finished mmult job!\n", _id);
         return true;
      }
      else {
         printf("%d received add result, %d to go!\n", _id, MAX_NUM_RECEIVED_ADD_RESULTS-num_received_add_results);
      }
   }
   //else {
   //   printf("%d received mmult job result\n", _id);
   //}

   if(value == 0){
      //printf("%d result is 0\n", _id);
      num_received_add_results++;
      return false;
   }

   // for both mul and add jobs
   if(result.data[pjob.matrix_field] == 0){
      result.data[pjob.matrix_field] = value;
      //printf("%d last result was 0\n", _id);
   }
   else {
      processor_job addjob;
      addjob.calculation_id = _id;
      addjob.matrix_field = pjob.matrix_field;
      addjob.data2 = value;
      addjob.data1 = result.data[pjob.matrix_field];
      addjob.type = JOB_TYPE_ADD;
      result.data[pjob.matrix_field] = 0;
      (*jobList).push_back(addjob);
      printf("%d added new add job, size: %lu\n", _id, (*jobList).size());
      addjob.debug_print();
   }

   return false;
}
