// Kevin Crane
//   CS531 - Homework 2
//   encrypt.c

#include <arpa/inet.h>
#include "encrypt.h"
#include "stream.h"


/*
 * Begin code I did not write.
 * Returns true if the current sys architecture uses big-endian notation for storing shorts and ints
 * The following code was obtained from: http://stackoverflow.com/a/1001330
 */
bool is_big_endian() {
	if ( htonl(47) == 47 )
		return true;
	else return false;
}
/*
 * End code I did not write.
 */


// Print the PBM headers to a file
void print_headers(FILE *out, int width, int height) {
	fprintf(out, "P4\n");
	fprintf(out, "%d %d\n", width, height);
}

// Print the two-row output for a given file
void print_rowout(FILE* out, unsigned char row_out[][2], int width) {
	for(int i=0; i<2; i++) {
		for(int j=0; j<width; j++) {
			fprintf(out, "%c", row_out[j][i]);
		}
	}
}

// Take bits read from input file, encrypt them with key_buf, and spit to new files
void encrypt_pbm_split_files(FILE* fout1, FILE* fout2, unsigned char *in_buf, unsigned char *key_buf, int width, int height) {
	unsigned char mask_in = 0x80;
	unsigned char mask_key = 0x80;
	unsigned char *curr_in;
	unsigned char *curr_key;
	unsigned short fstor1[2];	// = (unsigned int*)malloc(4 * sizeof(char));	// Four bytes to store bits (2 for each new row)
	unsigned short fstor2[2];	// = (unsigned int*)malloc(4 * sizeof(char));
	
	unsigned char row_out1[(2*width+7)/8][2];	// Function computes ceiling (2*width)/8 in bytes
	unsigned char row_out2[(2*width+7)/8][2];
	
	// Iterate through each row of the PBM file
	curr_in = (unsigned char*)in_buf;
	curr_key = (unsigned char*)key_buf;
	for(int i=0; i<height; i++) {
	
		// Iterate through each column, byte-wise
		for(int j=0; j<(width+7)/8; j++) {
		
			// Reset the encrypted bits for new file
			fstor1[0] = 0x0000;
			fstor1[1] = 0x0000;
			fstor2[0] = 0x0000;
			fstor2[1] = 0x0000;
			// Iterate through each bit of every byte in the row
			for(int k=0; k<8; k++) {
				if((j*8 + k) >= width) {
					if(k == 4) {
						// Printing don't cares from the start, just skip this byte
						curr_in++;
						mask_in = 0x80;
						break;
					}
				} else {
					// Identify whether the current bit is a 1 or 0
					if((unsigned int)(*curr_in & mask_in) > 0) {
						// Black pixel
						if((unsigned int)(*curr_key & mask_key) > 0) {
							// Black pixel, key=1
							fstor1[0] = fstor1[0] | (0x4000 >> (2*k));
							fstor1[1] = fstor1[1] | (0x8000 >> (2*k));
							fstor2[0] = fstor2[0] | (0x8000 >> (2*k));
							fstor2[1] = fstor2[1] | (0x4000 >> (2*k));
						} else {
							// Black pixel, key=0
							fstor1[0] = fstor1[0] | (0x8000 >> (2*k));
							fstor1[1] = fstor1[1] | (0x4000 >> (2*k));
							fstor2[0] = fstor2[0] | (0x4000 >> (2*k));
							fstor2[1] = fstor2[1] | (0x8000 >> (2*k));
						}
					} else {
						// White pixel
						if((unsigned int)(*curr_key & mask_key) > 0) {
							// White pixel, key=1
							fstor1[0] = fstor1[0] | (0x4000 >> (2*k));
							fstor1[1] = fstor1[1] | (0x8000 >> (2*k));
							fstor2[0] = fstor2[0] | (0x4000 >> (2*k));
							fstor2[1] = fstor2[1] | (0x8000 >> (2*k));
						} else {
							// White pixel, key=0
							fstor1[0] = fstor1[0] | (0x8000 >> (2*k));
							fstor1[1] = fstor1[1] | (0x4000 >> (2*k));
							fstor2[0] = fstor2[0] | (0x8000 >> (2*k));
							fstor2[1] = fstor2[1] | (0x4000 >> (2*k));
						}
					}
					
					// Update key mask
					mask_key = mask_key >> 1;
					if(mask_key == 0) {
						curr_key++;
						mask_key = 0x80;
					}
				}
				
				mask_in = mask_in >> 1;
				if(mask_in == 0) {
					curr_in++;
					mask_in = 0x80;
				}
			}
			
			// Save char values from the shorts in correct order, based on endianness
			if(is_big_endian()) {
				row_out1[j*2][0] = *(unsigned char*)&fstor1[0];
				row_out1[j*2+1][0] = *(((unsigned char*)&fstor1[0])+1);
				row_out1[j*2][1] = *(unsigned char*)&fstor1[1];
				row_out1[j*2+1][1] = *(((unsigned char*)&fstor1[1])+1);
				
				row_out2[j*2][0] = *(unsigned char*)&fstor2[0];
				row_out2[j*2+1][0] = *(((unsigned char*)&fstor2[0])+1);
				row_out2[j*2][1] = *(unsigned char*)&fstor2[1];
				row_out2[j*2+1][1] = *(((unsigned char*)&fstor2[1])+1);
			} else {
				// Little-endian
				row_out1[j*2][0] = *(((unsigned char*)&fstor1[0])+1);
				row_out1[j*2+1][0] = *(unsigned char*)&fstor1[0];
				row_out1[j*2][1] = *(((unsigned char*)&fstor1[1])+1);
				row_out1[j*2+1][1] = *(unsigned char*)&fstor1[1];
			
				row_out2[j*2][0] = *(((unsigned char*)&fstor2[0])+1);
				row_out2[j*2+1][0] = *(unsigned char*)&fstor2[0];
				row_out2[j*2][1] = *(((unsigned char*)&fstor2[1])+1);
				row_out2[j*2+1][1] = *(unsigned char*)&fstor2[1];
			}
			
		}

		// Output the two rows to each file
		print_rowout(fout1, row_out1, (2*width+7)/8);
		print_rowout(fout2, row_out2, (2*width+7)/8);
	}	
}

// Open two new PBM files for writing and begin encryption
void open_files(char *name, unsigned char *in_buf, unsigned char *key_buf, int width, int height) {
	int new_width, new_height;
	
	// Generate two filenames
	char name1[40];		// Arbitrary filename length
	char name2[40];
	strcpy(name1, name);
	strcpy(name2, name);
	
	// Open two new files for reading
	FILE *fout1 = fopen(strcat(name1, ".1.pbm\0"), "w");
	FILE *fout2 = fopen(strcat(name2, ".2.pbm\0"), "w");
	
	// Determine new file line width
	new_width = width * 2;
	new_height = height *2;
	
	// Print PBM file headers
	print_headers(fout1, new_width, new_height);
	print_headers(fout2, new_width, new_height);
	
	encrypt_pbm_split_files(fout1, fout2, in_buf, key_buf, width, height);
	
	// We're done here, close up shop
	fclose(fout1);
	fclose(fout2);
}


// Encrypt an input PBM file with a simple stream cipher
void encrypt(char *pphrase, char *name, FILE *in) {

	int width;
	int height;
	char *temp_buf = (char*)malloc(10);
	unsigned char *key_buf;
	unsigned char *in_buf;
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
	
	// Read all data from input file
	if(width%8 != 0) {
		in_buf = (unsigned char*)malloc(height * (width/8 + 1));
		fread(in_buf, 1, (height * (width/8 + 1)), in);
	} else {
		in_buf = (unsigned char*)malloc(height * width/8);
		fread(in_buf, 1, (height * width/8), in);
	}
	
	// Read stream cipher
	key_buf = (unsigned char*)malloc(height*(width/8 + 1));
	stream(pphrase, height*(width/8 + 1), key_buf);

	// Open two files for input
	open_files(name, in_buf, key_buf, width, height);
	
	// Free up the big memory buffers used
	free(in_buf);
	free(key_buf);
	
	
	// PSEUDO-CODE
	// - Read in the headers of the file, make sure "P4", width, and height are present
	// - Read in the entirety ot the PBM file into a large allocated buffer, in_buf
	// - Compute and store a sufficient portion of the stream cipher in a buffer, key_buf
	// - Open two new files and print the correct headers, with double the dimensions
	//   - Round up to nearest byte
	// - Define two 2-byte shorts to store the values for each new file that's splitting
	// - Define a 1-byte char mask that starts with value 0x80
	// - Iterate through the original PBM file, across each row by byte, then downwards
	//   - Read in a single byte (containing 8 bits) for bitwise operations
	//   - Repeating 8 times, identify whether the current bit is a 0 or 1 by AND'ing with the byte mask
	//   - Repeat the same thing with the key to determine whether the current key bit is a 0 or 1
	//   - Based off the two results above, set the appropriate bits to white (0) or black (1) in 2-byte short
	//   - Once a whole row has been, print all the values received to the two open split files
	// - Repeat until the whole file has been read and encrypted
}
