// Kevin Crane
//   CS531 - Homework 2
//   merge.c

#include <string.h>
#include <stdlib.h>
#include "merge.h"


// Take in 2 encrypted PBM files, bitwise OR them, write to fout
void merge_files(FILE *fin1, FILE *fin2, int width, int height) {
	unsigned char *val1 = (unsigned char*)malloc(1);
	unsigned char *val2 = (unsigned char*)malloc(1);
	unsigned char *val_out = (unsigned char*)malloc(1);
	
	// Iterate through all rows and columns
	for(int i=0; i<height; i++) {
		for(int j=0; j<(width+7)/8; j++) {
			// Read one byte from each file
			fread(val1, 1, 1, fin1);
			fread(val2, 1, 1, fin2);
			
			// OR the bytes read from both files, print
			*val_out = *val1 | *val2;
			printf("%c", *val_out);
		}
	}
	
	free(val1);
	free(val2);
	free(val_out);
}


// Merge 2 PBM files and create an output file
void merge(FILE *in1, FILE *in2, char* fname) {
	
	int width;
	int height;
	char *temp_buf = (char*)malloc(10);
	char magic_num[3];
	
	// Make sure the magic number (P4) of both PBM files is correct
	fgets(magic_num, 4, in1);
	if(strcmp(magic_num, "P4\n") != 0) {
		// Incorrect magic number
		fprintf(stderr, "ERROR: invalid PBM file1 for input.\n");
		return;
	}
	fgets(magic_num, 4, in2);
	if(strcmp(magic_num, "P4\n") != 0) {
		// Incorrect magic number
		fprintf(stderr, "ERROR: invalid PBM file2 for input.\n");
		return;
	}
	
	// Read height/width values from PBM file
	fgets(temp_buf, 10, in1);
	width = (int)strtol(temp_buf, &temp_buf, 10);
	height = (int)strtol(temp_buf, NULL, 10);
	fgets(temp_buf, 10, in2);
	if(width != (int)strtol(temp_buf, &temp_buf, 10) || height != (int)strtol(temp_buf, NULL, 10)) {
		// Height/width don't match
		fprintf(stderr, "ERROR: dimensions in file1 and file2 do not match.\n");
		return;
	}
	
	// Print Headers
	printf("P4\n");
	printf("%d %d\n", width, height);
	
	// Merge all the bits together and write to fout
	merge_files(in1, in2, width, height);
	
	
	// PSEUDO-CODE
	// - Read in 2 files and verify their headers
	// - Read each value in, byte by byte
	// - Bitwise OR each value and print it to the screen
	// * TO TEST THIS, DIRECT OUTPUT TO FILE
	//   e.g. ./hw2 merge money.1.pbm money.2.pbm > money-merged.pbm
}
