// Kevin Crane
//   CS531 - Homework 2
//   merge.h

#ifndef MERGE_H
#define MERGE_H

#include <stdio.h>
#include <string.h>


// Take in 2 encrypted PBM files, bitwise OR them, write to fout
void merge_files(FILE *fin1, FILE *fin2, FILE* fout, int width, int height);

// Merge 2 PBM files and create an output file
void merge(FILE *in1, FILE *in2, char *fname);

#endif
