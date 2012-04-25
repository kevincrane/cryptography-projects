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


// Test the primality of number using trial division by trying sequentially all small prime numbers
bool trialdiv(char* number, FILE* primesfile);

#endif
