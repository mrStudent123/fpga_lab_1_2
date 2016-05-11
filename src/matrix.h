/*
 * matrix.h
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#ifndef MATRIX_H_
#define MATRIX_H_

class matrix {

   short unsigned w;
   short unsigned h;
   short data[];

public:
   matrix(int w, int h);
   virtual ~matrix();

   bool put(int x, int y, short value);
   short get(int x, int y);

   void add(matrix m);
   void multiply(matrix m);
   void fillRandom(short max);
};

#endif /* MATRIX_H_ */
