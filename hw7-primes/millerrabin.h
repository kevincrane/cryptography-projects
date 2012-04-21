// Kevin Crane
//   CS531 - Homework 7
//   millerrabin.h

#ifndef MILLERRABIN_H
#define MILLERRABIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Test the primality of number using the Miller-Rabin algorithm
void millerrabin(char* number, int maxitr, FILE* primesfile);

#endif
