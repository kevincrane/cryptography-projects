// Kevin Crane
//   CS531 - Homework 7
//   trialdiv.h

#ifndef TRIALDIV_H
#define TRIALDIV_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <arpa/inet.h>


// Returns true if the character string is all digits
bool is_number(char* string);

// Returns true if num%factor == 0, using BigNum
bool is_factor(BIGNUM* num, unsigned int factor, BN_CTX* ctx);


// Test the primality of number using trial division by trying sequentially all small prime numbers
bool trialdiv(char* number, FILE* primesfile);

#endif
