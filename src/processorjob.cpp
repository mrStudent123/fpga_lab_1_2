/*
 * processorjob.cpp
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#include "processorjob.h"
#include "processor.h"

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

processor_instruction *processor_job::getInstructions(){

   processor_instruction instructions[2];
   if(type == JOB_TYPE_ADD){
      instructions[0].instruction = INSTR_SETA;
      instructions[0].data = data1;

      instructions[1].instruction = INSTR_ADD;
      instructions[1].data = data2;
   }
   else if(type == JOB_TYPE_MUL){
      instructions[0].instruction = INSTR_SETA;
      instructions[0].data = data1;

      instructions[1].instruction = INSTR_MUL;
      instructions[1].data = data2;
   }

   return instructions;
}

