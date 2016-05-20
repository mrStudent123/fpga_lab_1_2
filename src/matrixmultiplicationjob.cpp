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
   num_received_mul_results = 0;
   MAX_NUM_RECEIVED_ADD_RESULTS = 0;//result.w*result.h*(m1.w-1);
   MAX_NUM_RECEIVED_MUL_RESULTS = 0; //result.w*result.h*(m1.w);

   //printf("initial job list size %lu\n", (*jobList).size());

   for(int result_x=0; result_x < result.w; result_x++)
   {
      for(int result_y=0; result_y < result.h; result_y++)
      {
         for(int i=0; i<m1.w && i < m2.h; i++){
            /*if(m1.get(i, result_y) == 0 && m2.get(result_x,i) == 0){
               continue;
            }
            else if(m1.get(i, result_y) == 0){
               addToField(result_y * result.w + result_x, m2.get(result_x,i));
            }
            else if(m2.get(result_x,i) == 0) {
               addToField(result_y * result.w + result_x, m1.get(i, result_y));
            }
            else {*/
               MAX_NUM_RECEIVED_MUL_RESULTS++;
               processor_job job;
               job.calculation_id = _id;
               job.matrix_field = result_y * result.w + result_x;
               job.data2 = m1.get(i, result_y);
               job.data1 = m2.get(result_x,i);
               job.type = JOB_TYPE_MUL;
               (*jobList).push_back(job);
               //job.debug_print();
               //printf("Adding job to job list\n");
            //}
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
      //printf("%d received add job result %d\n", _id, value);
      num_received_add_results++;

      if(isFinished())
      {
         result.data[pjob.matrix_field] = value;
         //printf("%d finished mmult job!\n", _id);
         return true;
      }
      //else {
      //   printf("%d received add result, %d to go!\n", _id, MAX_NUM_RECEIVED_ADD_RESULTS-num_received_add_results);
      //}
   }
   else {
      //printf("%d received mmult job result %d\n", _id, value);
      num_received_mul_results++;
   }

   if(value == 0){
      //printf("%d result is 0\n", _id);
      return isFinished();
   }

   addToField(pjob.matrix_field, value);

   return false;
}

void matrix_multiplication_job::addToField(unsigned field, short value){
   // for both mul and add jobs
   if(result.data[field] == 0){
      result.data[field] = value;
      //printf("%d last result was 0\n", _id);
   }
   else {
      MAX_NUM_RECEIVED_ADD_RESULTS++;
      processor_job addjob;
      addjob.calculation_id = _id;
      addjob.matrix_field = field;
      addjob.data2 = value;
      addjob.data1 = result.data[field];
      addjob.type = JOB_TYPE_ADD;
      result.data[field] = 0;
      (*jobList).push_back(addjob);
      //printf("%d added new add job, size: %lu\n", _id, (*jobList).size());
      //addjob.debug_print();
   }
}

bool matrix_multiplication_job::isFinished(){
   return num_received_add_results >= MAX_NUM_RECEIVED_ADD_RESULTS &&
         num_received_mul_results >= MAX_NUM_RECEIVED_MUL_RESULTS;
}
