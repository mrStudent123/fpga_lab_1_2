/*
 * matrix.h
 *
 *  Created on: May 11, 2016
 *      Author: sclab64
 */

#ifndef MATRIX_H_
#define MATRIX_H_

class matrix {

public:
   short unsigned w;
   short unsigned h;
   short *data;

public:
   matrix();
   virtual ~matrix();

   void initialize(unsigned short w, unsigned short h);
   void initialize_value(matrix m);
   void initialize_value_twoxtwo(matrix m);
   void initializeRandom(unsigned short w, unsigned short h, short max);
   void initializeRandom_twoxtwo(short max);

   bool put(unsigned short x, unsigned short y, short value);
   short get(unsigned short x, unsigned short y);

   void add(matrix m);
   void multiply(matrix m);
   void multiply_twoxtwo(matrix m);
   void fillRandom(short max);

   bool equals(matrix m);

   void debug_print();
};

#endif /* MATRIX_H_ */
