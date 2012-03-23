// Kevin Crane
//   CS531 - Homework 2
//   decrypt.c

#include <string.h>
#include <stdlib.h>
#include "decrypt.h"


// Read in a file, print out decrypted bit values
void decrypt_file(FILE *fin, int width, int height) {
	unsigned char row_val1[(width+7)/8];
	unsigned char row_val2[(width+7)/8];
	unsigned char mask = 0xc0;
	unsigned char out_mask;

	unsigned char *val_out = (unsigned char*)malloc(1);
	
	// Iterate through all rows and columns
	for(int i=0; i<height; i+=2) {
		// Read two rows from the input
		fread(row_val1, 1, (width+7)/8, fin);
		fread(row_val2, 1, (width+7)/8, fin);
	
		for(int j=0; j<(width+7)/8; j++) {
			if(j%2 == 0) {
				*val_out = 0x00;
				out_mask = 0x80;
			} else {
				out_mask = 0x08;
			}
			
			// Read the first byte from the row, 2 bits at a time
			for(int k=0; k<4; k++) {
				// Checks if both current bits from both lines are black
				if(((row_val1[j] & (mask >> 2*k)) == (mask >> 2*k)) &&
						((row_val2[j] & (mask >> 2*k)) == (mask >> 2*k))) {
					// Black pixel
					*val_out = *val_out | (out_mask >> k);
				}
			}
			
			if(j%2 == 1 || ((j+1)*8 + 4 >= width)) {
				printf("%c", *val_out);
			}
		}
	}
	
	free(val_out);
}


// Decrypt a merged input PBM file, returning new PBM file
void decrypt(FILE *in) {

	int width;
	int height;
	char *temp_buf = (char*)malloc(10);
	char magic_num[3];

	// Make sure the magic number (P4) of the PBM file is correct
	fgets(magic_num, 4, in);
	if(strcmp(magic_num, "P4\n") != 0) {
		// Incorrect magic number
		fprintf(stderr, "ERROR: invalid PBM file for input.\n");
		return;
	}
	
	// Read height/width values from PBM file
	fgets(temp_buf, 10, in);
	width = (int)strtol(temp_buf, &temp_buf, 10);
	height = (int)strtol(temp_buf, NULL, 10);
	
	// Create a new file
	printf("P4\n");
	printf("%d %d\n", width/2, height/2);
	
	decrypt_file(in, width, height);
	
	// PSEUDO-CODE
	// - Read in the file and verify its header
	// - Print PBM file header
	// - Repeat through all the lines:
	//   - Read two full lines of bytes
	//   - Take one byte from both lines
	//   - Use mask 0xC0 shifted appropriately to determine
	//     if there are two black bits on each line
	//   - If so, flip a bit in out_val to 1 in correct spot
	//   - Repeat until line complete, then print char out_val
}
