// Kevin Crane
//   CS531 - Homework 6
//   inverse.h

#ifndef INVERSE_H
#define INVERSE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "modprod.h"
#include "tablecheck.h"


// Initialize 256-char table of multiplicative inverses
//	through AES' finite fielld (mod x^4 + 1)
void init_inv_table(unsigned char inv_table[256]);

// Calculate the multiplicative inverse of the polynomial with coefficients in GF(2^8)
//	Store the results in inv_poly (input: char array of 4 bytes as coefficients)
void get_inverse(unsigned char poly[4], unsigned char inv_table[256], unsigned char inv_poly[4]);

// Calculate the multiplicative inverse of the polynomial with coefficients in GF(2^8)
void inverse(char* poly);

#endif
