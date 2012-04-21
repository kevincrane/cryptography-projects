// Kevin Crane
//   CS531 - Homework 7
//   maurer.h

#ifndef MAURER_H
#define MAURER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Generate a numbits-bit provable prime using the Maurer algorithm
void maurer(int numbits, FILE* primesfile, FILE* rndfile);

#endif
