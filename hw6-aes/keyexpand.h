// Kevin Crane
//   CS531 - Homework 6
//   keyexpand.h

#ifndef KEYEXPAND_H
#define KEYEXPAND_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Perform the key expansion for AES-128 using key as 16-byte string in hexstring format
void keyexpand(char* key, FILE* table);

#endif
