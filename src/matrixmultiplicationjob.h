/*
 * matrixmultiplicationjob.h
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#ifndef MATRIXMULTIPLICATIONJOB_H_
#define MATRIXMULTIPLICATIONJOB_H_

#include "matrix.h"
#include "processorjob.h"
#include <list>

class matrix_multiplication_job {
private:
   matrix result;
   std::list<processor_job> jobList;
   unsigned num_received_add_results;
   unsigned MAX_NUM_RECEIVED_ADD_RESULTS;
public:
   unsigned _id;
   matrix m1;
   matrix m2;

   matrix_multiplication_job(unsigned id, matrix matrix1, matrix matrix2);
   virtual ~matrix_multiplication_job();

   processor_job getJob();
   bool hasJobs();

   // returns true if multiplication is finished
   bool putJobResult(processor_job job, short value);

   matrix getResult();
};

#endif /* MATRIXMULTIPLICATIONJOB_H_ */
