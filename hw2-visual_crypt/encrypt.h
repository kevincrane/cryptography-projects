// Kevin Crane
//   CS531 - Homework 2
//   encrypt.h

#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// Print the PBM headers to a file
void print_headers(FILE *out, int width, int height);

// Print the two-row output for a given file
void print_rowout(FILE* out, unsigned char row_out[][2], int width);

// Take bits read from input file, encrypt them with key_buf, and spit to new files
void encrypt_pbm_split_files(FILE* fout1, FILE* fout2, unsigned char *in_buf, unsigned char *key_buf, int width, int height);

// Open two new PBM files for writing and begin encryption
void open_files(char *name, unsigned char *in_buf, unsigned char *key_buf, int width, int height);

// Encrypt an input PBM file with a simple stream cipher
void encrypt(char *pphrase, char *name, FILE *in);

#endif
