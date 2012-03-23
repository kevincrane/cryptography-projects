// Kevin Crane
//   CS531 - Homework 4
//   crypt.h

#ifndef CRYPT_H
#define CRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Encrypts an input ASCII text file with the keys specified in keyfile
void crypt(FILE *key, FILE *text);

#endif
