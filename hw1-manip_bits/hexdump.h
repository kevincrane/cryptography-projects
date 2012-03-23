// Kevin Crane
//   CS531 - Homework 1
//   hexdump.h

#ifndef HEXDUMP_H
#define HEXDUMP_H

#include <stdio.h>
#include <string.h>


// Prints plaintext output at end of row
void printLineOut(char lineOut[]);

// Complete a hexdump of ASCII characters received
void hexdump(FILE*);

#endif
