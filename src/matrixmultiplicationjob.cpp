/*
 * matrixmultiplicationjob.cpp
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#include "matrixmultiplicationjob.h"
#include "processorjob.h"

matrix_multiplication_job::matrix_multiplication_job(unsigned id, matrix matrix1, matrix matrix2) {

   _id = id;
   m1 = matrix1;
   m2 = matrix2;

   result.initialize(m1.h, m2.w);
   number_of_finished_fields = 0;

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

   if(true){
      //type == MUL
   }
   else if(true){
      //type == ADD
   }

   /*if(pjob.type == JOB_TYPE_MUL)
   {
      //TODO: create add job
   }
   else if(job.type == JOB_TYPE_ADD){
      //TODO: check if it was the last addition, if so write value to field and (if last field) return true, otherwise create new add job
   }*/

   return false;
}
