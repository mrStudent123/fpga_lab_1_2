/*
 * matrix.cpp
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#include "matrix.h"
#include <stdlib.h> /* fuer zufallszahlen */
#include <stdio.h>

matrix::matrix() {
   data = 0;
   w=0;
   h=0;
}

matrix::~matrix() {
   // TODO Auto-generated destructor stub
}

void matrix::initialize(unsigned short width, unsigned short height) {
   w = width;
   h = height;

   data = new short[w*h];

   for(int i=0; i<w*h; i++){
      data[i] = 0;
   }
}

void matrix::initialize_value(matrix m) {
   w = m.w;
   h = m.h;

   data = new short[w*h];

   for(int i=0; i<w*h; i++){
      data[i] = m.data[i];
   }
}

void matrix::initializeRandom(unsigned short width, unsigned short height, short max){
   w = width;
   h = height;

   data = new short[width*height];

   fillRandom(max);
}

short matrix::get(unsigned short x, unsigned short y){
   if(x > w || y > h){
      return 0;
   }

   return data[y*w + x];
}

bool matrix::put(unsigned short x, unsigned short y, short value){
   if(x>w || y > h){
      return false;
   }

   data[y*w + x] = value;
   return true;
}

void matrix::add(matrix m){
   for(int i=0; i<h*w; i++){
      data[i] += m.data[i];
   }
}

matrix matrix::multiply(matrix m){
   matrix result;
   result.initialize(m.w, h);

   for(int result_x=0; result_x < result.w; result_x++)
   {
      for(int result_y=0; result_y < result.h; result_y++)
      {
         for(int i=0; i<w && i < m.h; i++){
            result.data[result_y*result.w + result_x] += get(i,result_y) * m.get(result_x,i);
         }
      }
   }

   return result;
}

void matrix::fillRandom(short max){
   for(short i=0; i<w*h; i++){
      data[i] = (short)((rand() % (max*2)) - max);
   }

   //printf("random fill:\n");
   //debug_print();
}

bool matrix::equals(matrix m){
   if(w != m.w || h != m.h){
      return false;
   }

   for(int i=0; i<h*w; i++ ){
      if(data[i] != m.data[i]){
         return false;
      }
   }

   return true;
}

void matrix::debug_print(){
   //printf("matrix data pointer %p\n", data);
   for(int i=0; i< w*h; i++){
      printf("%4hd ", data[i]);
      if((i+1)%w == 0){
         printf("\n");
      }
   }
   printf("\n");
}
