// Kevin Crane
//   CS531 - Homework 7
//   rndsearch.h

#ifndef RNDSEARCH_H
#define RNDSEARCH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <openssl/crypto.h>
#include <openssl/bn.h>


// Takes in a BigNum, sets bits 0 and k-1 to 1, bits >= k to 0
void RndOddNum(int numbits, BIGNUM* odd_num, FILE* rndfile);

// Generate a numbits-bit probable prime using the Random-Search algorithm 
void rndsearch(int numbits, int maxitr, FILE* primesfile, FILE* rndfile);

#endif
