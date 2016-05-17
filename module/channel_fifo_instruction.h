#include "systemc.h"
#include "../src/processor_instruction.h"
#include "processor.h"

#ifndef PROCESSOR_CHANNEL_FIFO
#define PROCESSOR_CHANNEL_FIFO

#define INSTRUCTION_FIFO_SIZE 10

class if_fifo_instruction_in : virtual public sc_interface {
public:
   virtual bool putItem(processor_instruction sample) = 0;
};

class if_fifo_instruction_out : virtual public sc_interface {
public:
   virtual processor_instruction getItem() = 0;
   virtual bool hasItems() = 0;
   virtual unsigned num_free() = 0;
};

class channel_fifo_instruction: public sc_prim_channel,
      public if_fifo_instruction_in, public if_fifo_instruction_out
{
public:

   processor_instruction items[INSTRUCTION_FIFO_SIZE];
   unsigned short count;
   unsigned short index;

   channel_fifo_instruction(){
      count = 0;
      index = 0;
   }

   virtual bool putItem(processor_instruction val ){
      if(count >= INSTRUCTION_FIFO_SIZE){
         return false;
      }

      items[(index + count) % INSTRUCTION_FIFO_SIZE] = val;
      count++;

      return true;
   }

   virtual unsigned num_free(){
      return INSTRUCTION_FIFO_SIZE - count;
   }

  virtual processor_instruction getItem(){
     if(count > 0){
       unsigned oldIndex = index;
       unsigned newIndex = index;

       if(count == 1){
          newIndex = 0;
       }
       else {
          newIndex = (newIndex + 1) % INSTRUCTION_FIFO_SIZE;
       }

       processor_instruction value = items[oldIndex];
       count --;
       index = newIndex;
       return value;
    }

     processor_instruction i;
     return i;
  }

  virtual bool hasItems(){
     return count > 0;
  }
};

#endif
