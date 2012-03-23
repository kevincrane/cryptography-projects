// Kevin Crane
//   CS531 - Homework 2
//   decrypt.h

#ifndef DECRYPT_H
#define DECRYPT_H

#include <stdio.h>
#include <string.h>


// Read in a file, print out decrypted bit values
void decrypt_file(FILE *fin, int width, int height);

// Decrypt a merged input PBM file, returning new PBM file
void decrypt(FILE *in);

#endif
