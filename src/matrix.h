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
   matrix();
   virtual ~matrix();

   void initialize(unsigned short w, unsigned short h);
   void initializeRandom(unsigned short w, unsigned short h, short max);

   bool put(unsigned short x, unsigned short y, short value);
   short get(unsigned short x, unsigned short y);

   void add(matrix m);
   void multiply(matrix m);
   void fillRandom(short max);
};

#endif /* MATRIX_H_ */
