// Kevin Crane
//   CS531 - Homework 1
//   enc_base64.c

#include "enc_base64.h"

// Defines the base64 alphabet that must be used to display base64 characters
char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


// Encode ASCII values in input received to Base64
void enc_base64(FILE *in) {
	char buf[3];
	unsigned char curr[3];
	unsigned char out[4];
	int bytes_read;
	int column_count = 0;
	
	// Initial reading of input
	bytes_read = fread(buf, 1, 3, in);
	
	// Iterate through the entire input by bytes
	while(bytes_read > 0) {
		for(int i=0; i<3; i++) {
			curr[i] = buf[i];
		}
		
		// Has stored three characters, convert to base64
		out[0] = alpha[ curr[0] >> 2 ];
		out[1] = alpha[ ((curr[0] & 0x03) << 4) | ((curr[1] & 0xf0) >> 4) ];
		if(bytes_read > 1) {
			out[2] = alpha[ ((curr[1] & 0x0f) << 2) | ((curr[2] & 0xc0) >> 6) ];
		} else {
			out[2] = '=';
		}
		if(bytes_read > 2) {
			out[3] = alpha[ curr[2] & 0x3f ];
		} else {
			out[3] = '=';
		}
		
		// Print the output to the screen as we go, newline after 64 chars
		for(int i=0; i<4; i++) {
			printf("%c", out[i]);
			column_count++;
			if(column_count >= 64) {
				printf("\n");
				column_count = 0;
			}
		}
		
		// Clear existing buffer and read next block in
		memset(buf, 0, sizeof(char)*3);
		if(bytes_read > 0)
			bytes_read = fread(buf, 1, 3, in);
	}
	
	// Append newline to output, in case one is needed
	if(column_count != 0) {
		printf("\n");
	}
	
	// PSEUDO-CODE
	// - Read 3 ASCII bytes at a time
	// - Do bit-wise operations to drop from 8 bits/char -> 6 bits/char
	// - Do lookup to see what Base64 matches the 6 bit value that just got made
	// - Concat each new Base64 char to output
	// * If not enough bytes read, concat '=' to end
}
