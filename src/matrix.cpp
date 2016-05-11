/*
 * matrix.cpp
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#include "matrix.h"
#include <stdlib.h> /* fuer zufallszahlen */

matrix::matrix(unsigned short width, unsigned short height) {
   w = width;
   h = height;

   short matrix_data[width*height];
   data = matrix_data;

   for(int i=0; i<w*h; i++){
      data[i] = 0;
   }
}

matrix::~matrix() {
   // TODO Auto-generated destructor stub
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
   for(int i=0; i<m*w; i++){
      data[i] += m.data[i];
   }
}

void matrix::multiply(matrix m){
   matrix result(h,m.w);

   for(int result_x=0; result_x < result.w; result_x++)
   {
      for(int result_y=0; result_y < result.h; result_y++)
      {
         for(int i=0; i<w && i < m.h; i++){
            result.data[result_y*result.w + result_x] += get(result_y,i) * m.get(i,result_x);
         }
      }
   }
}

void matrix::fillRandom(short max){
   for(int i=0; i<w*h; i++){
      data[i] = rand() % max;
   }
}
