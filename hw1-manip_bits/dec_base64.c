// Kevin Crane
//   CS531 - Homework 1
//   dec_base64.c

#include "dec_base64.h"


// Encode Base64 values in input received to ASCII
void dec_base64(FILE *in) {
	char buf[1];
	unsigned char curr[4];
	unsigned char out[3];
	int bytes_read = 0;
	bool done = false;
	
	// Iterate through the entire input by bytes
	while(fread(buf, 1, 1, in) > 0) {
	
		// Check to see if byte read is valid input, align it with Base64 alphabet if it is
		if(buf[0] == '\n') {
			continue;
		} else if(buf[0] >= 'A' && buf[0] <= 'Z') {
			curr[bytes_read] = buf[0] - 'A';
		} else if(buf[0] >= 'a' && buf[0] <= 'z') {
			curr[bytes_read] = buf[0] - 'a' + 0x1a;
		} else if(buf[0] >= '0' && buf[0] <= '9') {
			curr[bytes_read] = buf[0] - '0' + 0x34;
		} else if(buf[0] == '+') {
			curr[bytes_read] = 0x3e;
		} else if(buf[0] == '/') {
			curr[bytes_read] = 0x3f;
		} else if(buf[0] == '=') {
			// Reached end of input stream, indicate ending
			curr[bytes_read] = 0;
			done = true;
		} else {
			fprintf(stderr, "ERROR: input stream contained invalid Base64 characters (%c).\n", buf[0]);
			return;
		}
		if(!done)
			bytes_read++;
		
		// Check if enough bytes have been read to convert this block back to ASCII
		if(bytes_read == 4 || done) {
			// Convert the 4 Base64 characters to ASCII bytes
			out[0] = (curr[0] << 2) | ((curr[1] & 0x30) >> 4);
			if(bytes_read >= 3)
				out[1] = ((curr[1] & 0x0f) << 4) | ((curr[2] & 0x3c) >> 2);
			if(bytes_read == 4)
				out[2] = ((curr[2] & 0x03) << 6) | (curr[3] & 0x3f);
	
			for(int i=0; i<(bytes_read-1); i++) {
				printf("%c", out[i]);
			}
			
			if(done) {
				// Hit end, done
				break;
			} else {
				curr[0] = curr[1] = curr[2] = curr[3] = 0;
				bytes_read = 0;
			}
		}
		
	}
	
	// PSEUDO-CODE
	// - Read 4 chars from input
	// - If between A-Z, subtract 'A'; if between a-z, subtract ('a'-26); if '+' -> 62; if '/' -> 63
	// - Now have 4 bytes of values 0-63. Do bit operations to convert to 3 8-bit values
	// - Print them out
	// * If you get '\n' in buffer, skip it
	// * If you get '=' in buffer, end. If not enough values in buffer, add 0's?
	// * If value not in ranges listed above, print to stderr that the input file does not contain valid Base64 characters, end program
}
