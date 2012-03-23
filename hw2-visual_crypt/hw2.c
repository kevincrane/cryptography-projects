// Kevin Crane
//   CS531 - Homework 2
//   Introduction to Visual Cryptography

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stream.h"
#include "encrypt.h"
#include "merge.h"
#include "decrypt.h"


// Called if invalid parameters are initially called
void usage(const char *type="none") {
	if(strcmp(type, "none") == 0) {
		fprintf(stderr, "ERROR: invalid parameters. Should be one of the following:\n");
		fprintf(stderr, "\thw2 stream -p=pphrase -l=len\n");
		fprintf(stderr, "\thw2 encrypt -p=pphrase -out=name [pbmfile]\n");
		fprintf(stderr, "\thw2 merge pbmfile1 pbmfile2\n");
		fprintf(stderr, "\thw2 decrypt [pbmfile]\n");
	} else {
		fprintf(stderr, "ERROR: invalid parameters. Should be the following:\n");
		if(strcmp(type, "stream") == 0) {
			fprintf(stderr, "\thw2 stream -p=pphrase -l=len\n");
		} else if(strcmp(type, "encrypt") == 0) {
			fprintf(stderr, "\thw2 encrypt -p=pphrase -out=name [pbmfile]\n");
		} else if(strcmp(type, "merge") == 0) {
			fprintf(stderr, "\thw2 merge pbmfile1 pbmfile2\n");
		} else if(strcmp(type, "decrypt") == 0) {
			fprintf(stderr, "\thw2 decrypt [pbmfile]\n");
		}
	}
	return;
}

// Main method of HW2, mostly taking user input and running the correct prog
int main(int argc, char *argv[]) {

	// Look for correct command line arguments
	if(argc <= 1) {
		usage();
		return 0;
	} else {
		if(strcmp(argv[1], "stream") == 0) {
			// Stream cipher prog should be run
			char* pphrase = NULL;
			int len = -1;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-p=", 3) == 0) {
					pphrase = argv[i]+3;
				} else if(strncmp(argv[i], "-l=", 3) == 0) {
					len = atoi(argv[i]+3);
				}
			}
			if(pphrase && len > 0) {
				stream(pphrase, len, NULL);
			} else {
				usage("stream");
			}
		}
		else if(strcmp(argv[1], "encrypt") == 0) {
			// Encrypt PBM prog should be run
			char* pphrase = NULL;
			char* name = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-p=", 3) == 0) {
					pphrase = argv[i]+3;
				} else if(strncmp(argv[i], "-out=", 5) == 0) {
					name = argv[i]+5;
				}
			}
			if(pphrase && name) {
				if(argv[argc-1][0] != '-') {
					// Take input from file
					FILE *fin = NULL;
					fin = fopen(argv[argc-1], "r");
					if(fin == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-1]);
						return 0;
					}
					encrypt(pphrase, name, fin);
					fclose(fin);
				} else {
					// Take input from cmd line
					encrypt(pphrase, name, stdin);
				}
			} else {
				usage("encrypt");
			}
		}
		else if(strcmp(argv[1], "merge") == 0) {
			// Merge PBMs progr should run
			if(argc != 4) {
				usage("merge");
				return 0;
			} else {
				// Open two PBM files for input
				FILE *pbmfile1 = NULL;
				FILE *pbmfile2 = NULL;
				pbmfile1 = fopen(argv[argc-2], "r");
				pbmfile2 = fopen(argv[argc-1], "r");
				if(pbmfile1 == NULL) {
					fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-2]);
					return 0;
				}
				if(pbmfile2 == NULL) {
					fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-2]);
					return 0;
				}
				
				// Get filename; find root, append null char
				char* ptr = strstr(argv[argc-2], ".1.pbm");
				strncpy(ptr, "\0", 1);
				
				merge(pbmfile1, pbmfile2, argv[argc-2]);
				fclose(pbmfile2);
				fclose(pbmfile1);
			}
		}
		else if(strcmp(argv[1], "decrypt") == 0) {
			// Decrypt PBM prog should run
			if(argc == 3) {
				// Take input from file
				FILE *fin = NULL;
				fin = fopen(argv[2], "r");
				if(fin == NULL) {
					fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[2]);
					return 0;
				}
				decrypt(fin);
				fclose(fin);
			} else {
				// Take input from cmd line
				decrypt(stdin);
			}
		}
		else {
			// Failed input
			usage();
		}
	}
	
	return 0;
}

