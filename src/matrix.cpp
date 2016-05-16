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

   data = new short[width*height];

   for(int i=0; i<w*h; i++){
      data[i] = 0;
   }
}

void matrix::initialize_value(matrix m) {

   if(w != m.w || h != m.h){
      printf("w,h: %hd %hd %hd %hd\n", w, h, m.w, m.h);
      printf("error in matrix::initialize_value\n");
      abort();
   }

   for(int i=0; i<w*h; i++){
      data[i] = m.data[i];
   }
}

void matrix::initialize_value_twoxtwo(matrix m) {

   if(w != 2 || h != 2 || m.w != 2 || m.h != 2){
      printf("error in matrix::initialize_value_twoxtwo\n");
      abort();
   }

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

void matrix::initializeRandom_twoxtwo(short max){

   if(w != 2 || h != 2){
      printf("error in matrix::initializeRandom_twoxtwo\n");
      abort();
   }

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

void matrix::multiply(matrix m){
   matrix result;
   result.initialize(h, m.w);

   for(int result_x=0; result_x < result.w; result_x++)
   {
      for(int result_y=0; result_y < result.h; result_y++)
      {
         for(int i=0; i<w && i < m.h; i++){
            result.data[result_y*result.w + result_x] += get(result_y,i) * m.get(i,result_x);
         }
      }
   }

   //for(int j=0; j<w && j < m.h; j++){
   //   data[j] = result.data[j];
   //}
}

void matrix::multiply_twoxtwo(matrix m){
   matrix temp;
   temp.initialize(2,2);

   matrix result;
   result.initialize(2, 2);

   if(w != 2 || h != 2){
      printf("error in multiply_twoxtwo\n");
      abort();
   }

   temp.data[0] = data[0];
   temp.data[1] = data[1];
   temp.data[2] = data[2];
   temp.data[3] = data[3];

   result.data[0] = temp.data[0]*m.data[0] + temp.data[1]*m.data[2];
   result.data[1] = temp.data[0]*m.data[1] + temp.data[1]*m.data[3];
   result.data[2] = temp.data[2]*m.data[0] + temp.data[3]*m.data[2];
   result.data[3] = temp.data[2]*m.data[1] + temp.data[3]*m.data[3];

   //printf("a: %hd, %hd, %hd, %hd \n", temp.data[0], temp.data[1], temp.data[2], temp.data[3]);
   //printf("b: %hd, %hd, %hd, %hd \n", m.data[0], m.data[1], m.data[2], m.data[3]);
   //printf("axb: %hd, %hd, %hd, %hd \n", result.data[0], result.data[1], result.data[2], result.data[3]);

   data[0] = result.data[0];
   data[1] = result.data[1];
   data[2] = result.data[2];
   data[3] = result.data[3];

}

void matrix::fillRandom(short max){
   for(short i=0; i<w*h; i++){
      data[i] = (short)((rand() % (max*2)) - max);
   }

   //printf("random fill:\n");
   debug_print();
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
      printf("%2hd ", data[i]);
   }
   printf("\n");
}
