/*
 * processorjob.h
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#ifndef PROCESSORJOB_H_
#define PROCESSORJOB_H_

#include "processor_instruction.h"

const unsigned short JOB_TYPE_NONE = 0;
const unsigned short JOB_TYPE_ADD = 1;
const unsigned short JOB_TYPE_MUL = 2;

class processor_job {
public:
   unsigned calculation_id;
   unsigned short type;
   unsigned short matrix_field;
   short data1;
   short data2;

   processor_job();
   virtual ~processor_job();

   processor_instruction *getInstructions();
   void debug_print();
};

#endif /* PROCESSORJOB_H_ */
