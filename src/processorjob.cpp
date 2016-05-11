/*
 * processorjob.cpp
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#include "processorjob.h"

processor_job::processor_job() {
   type = JOB_TYPE_NONE;
   calculation_id = 0;
   matrix_field = 0;
   data1 = 0;
   data2 = 0;
}

processor_job::~processor_job() {
   // TODO Auto-generated destructor stub
}

