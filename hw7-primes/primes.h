// Kevin Crane
//   CS531 - Homework 7
//   primes.h

#ifndef PRIMES_H
#define PRIMES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>


// Print long unsigned int as bytes
void print_int_bytes(unsigned int val);

// Print all primes between 2 and maxval, inclusive
void sieve(unsigned int maxval);


// Generate all prime numbers less than or equal to maxval. 
//	maxval must be between 2 and 224, inclusive.
void primes(unsigned int maxval);

#endif
