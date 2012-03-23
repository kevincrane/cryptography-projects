// Kevin Crane
//   CS531 - Homework 1
//   Hex dump of input & encode/decode to Base64

#include <stdio.h>
#include <string.h>

#include "hexdump.h"
#include "enc_base64.h"
#include "dec_base64.h"


// Main method of HW1
int main(int argc, char *argv[]) {

	// Look for correct command line arguments
	if(argc <= 1 || argc > 3) {
		fprintf(stderr, "ERROR: insufficient parameters. Should be: 'hw1 (hexdump/enc-base64/dec-base64) [file]'\n");
		return 0;
	} else {
		if(strcmp(argv[1], "hexdump") == 0) {
			// Hexdump prog should be run
			if(argc == 3) {
				// Take input from file
				FILE *fin = NULL;
				fin = fopen(argv[2], "r");
				if(fin == NULL) {
					fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[2]);
					return 0;
				}
				hexdump(fin);
				fclose(fin);
			} else {
				// Take input from cmd line
				hexdump(stdin);
			}
		}
		else if(strcmp(argv[1], "enc-base64") == 0) {
			// Encode to Base64 prog should run
			if(argc == 3) {
				// Take input from file
				FILE *fin = NULL;
				fin = fopen(argv[2], "r");
				if(fin == NULL) {
					fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[2]);
					return 0;
				}
				enc_base64(fin);
				fclose(fin);
			} else {
				// Take input from cmd line
				enc_base64(stdin);
			}
		}
		else if(strcmp(argv[1], "dec-base64") == 0) {
			// Decode from Base64 prog should run
			if(argc == 3) {
				// Take input from file
				FILE *fin = NULL;
				fin = fopen(argv[2], "r");
				if(fin == NULL) {
					fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[2]);
					return 0;
				}
				dec_base64(fin);
				fclose(fin);
			} else {
				// Take input from cmd line
				dec_base64(stdin);
			}
		}
		else {
			fprintf(stderr, "ERROR: invalid parameters. Should be: 'hw1 (hexdump/enc-base64/dec-base64) [file]'\n");
			return 0;
		}
	}
	
	return 0;
}

