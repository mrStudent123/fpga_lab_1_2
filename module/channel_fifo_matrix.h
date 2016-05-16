#include "systemc.h"
#include "../src/matrix.h"


#ifndef MATRIX_CHANNEL_FIFO
#define MATRIX_CHANNEL_FIFO

#define MATRIX_FIFO_SIZE 10

class if_fifo_matrix_in : virtual public sc_interface {
public:
   virtual bool putItem(matrix sample) = 0;
   virtual unsigned num_free() = 0;
};

class if_fifo_matrix_out : virtual public sc_interface {
public:
   virtual matrix getItem() = 0;
   virtual bool hasItems() = 0;
};

class channel_fifo_matrix: public sc_prim_channel,
            public if_fifo_matrix_in, public if_fifo_matrix_out
{
public:

   matrix items[MATRIX_FIFO_SIZE];
   unsigned short count;
   unsigned short index;

   channel_fifo_matrix(){
      printf("matrix channel constructor\n");
      count = 0;
      index = 0;
   }

   virtual bool putItem(matrix val ){
      //printf("putItem: count, index: %hd, %hd\n", count, index);
      if(count >= MATRIX_FIFO_SIZE){
         return false;
      }

      items[(index + count) % MATRIX_FIFO_SIZE] = val;
      count++;

      return true;
   }

   virtual unsigned num_free(){
         return INSTRUCTION_FIFO_SIZE - count;
   }

  virtual matrix getItem(){
     //printf("getItem: count, index: %hd, %hd\n", count, index);
     if(count > 0){
        count --;

        unsigned oldIndex = index;

        if(count == 0){
           index = 0;
        }
        else {
           index = (index + 1) % MATRIX_FIFO_SIZE;
        }

        return items[oldIndex];
     }

     matrix i;
     return i;
  }

  virtual bool hasItems(){
     return count > 0;
  }
};

#endif
