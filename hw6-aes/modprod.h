// Kevin Crane
//   CS531 - Homework 6
//   modprod.h

#ifndef MODPROD_H
#define MODPROD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Returns the ceiling of log2(n) as an integer (2^x = n; return ceiling of x)
int log2(int n);

// Returns the char result of coef1 x coef2
unsigned char mult(unsigned char coef1, unsigned char coef2);

// Stores a char* to an 8-char array, containing hexchars as 4 byte coefficients in out_coef
void get_modprod(unsigned char* p1, unsigned char* p2, unsigned char out_coef[4]);

// Perform modular product of two polynomials in GF(2^8)
void modprod(char* poly1, char* poly2);

#endif
