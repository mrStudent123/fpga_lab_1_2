/*
 * processorjob.h
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#ifndef PROCESSORJOB_H_
#define PROCESSORJOB_H_

#define JOB_TYPE_NONE 0;
#define JOB_TYPE_ADD 1;
#define JOB_TYPE_MUL 2;

class processor_job {
public:
   unsigned calculation_id;
   unsigned short type;
   unsigned short matrix_field;
   short data1;
   short data2;

   processor_job();
   virtual ~processor_job();
};

#endif /* PROCESSORJOB_H_ */
