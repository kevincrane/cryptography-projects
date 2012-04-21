// Kevin Crane
//   CS531 - Homework 7
//   rndsearch.h

#ifndef RNDSEARCH_H
#define RNDSEARCH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Generate a numbits-bit probable prime using the Random-Search algorithm 
void rndsearch(int numbits, int maxitr, FILE* primesfile, FILE* rndfile);

#endif
