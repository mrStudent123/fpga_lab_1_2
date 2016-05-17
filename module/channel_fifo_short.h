#include "systemc.h"


#ifndef SHORT_CHANNEL_FIFO
#define SHORT_CHANNEL_FIFO

#define SHORT_FIFO_SIZE 10

class if_fifo_short_in : virtual public sc_interface {
public:
   virtual bool putItem(short sample) = 0;
   virtual unsigned num_free() = 0;
};

class if_fifo_short_out : virtual public sc_interface {
public:
   virtual short getItem() = 0;
   virtual bool hasItems() = 0;
};

class channel_fifo_short: public sc_prim_channel,
            public if_fifo_short_in, public if_fifo_short_out
{
public:

   short items[SHORT_FIFO_SIZE];
   unsigned short count;
   unsigned short index;

   short temp_trace;    //for tracefile

   channel_fifo_short(){
      count = 0;
      index = 0;
   }

   virtual bool putItem(short val ){
      //printf("putItem: count, index: %hd, %hd\n", count, index);
      if(count >= SHORT_FIFO_SIZE){
         return false;
      }

      items[(index + count) % SHORT_FIFO_SIZE] = val;
      count++;

      return true;
   }

   virtual unsigned num_free(){
         return SHORT_FIFO_SIZE - count;
   }

  virtual short getItem(){
     //printf("getItem: count, index: %hd, %hd\n", count, index);
     if(count > 0){
        unsigned oldIndex = index;
        unsigned newIndex = index;

        if(count == 1){
           newIndex = 0;
        }
        else {
           newIndex = (newIndex + 1) % SHORT_FIFO_SIZE;
        }

        short value = items[oldIndex];
        count --;
        index = newIndex;

        temp_trace = value;

        return value;
     }

     return 0;
  }

  virtual bool hasItems(){
     return count > 0;
  }
};

#endif
