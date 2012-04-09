// Kevin Crane
//   CS531 - Homework 6
//   AES (Advanced Encryption Standard)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablecheck.h"
#include "modprod.h"
#include "keyexpand.h"
#include "encrypt.h"
#include "decrypt.h"
#include "inverse.h"


// Called if invalid parameters are initially called
void usage(const char *type="none") {
	if(strcmp(type, "none") == 0) {
		fprintf(stderr, "ERROR: invalid parameters. Should be one of the following:\n");
		fprintf(stderr, "\thw6 tablecheck -t=tablefile\n");
		fprintf(stderr, "\thw6 modprod -p1=poly1 -p2=poly2\n");
		fprintf(stderr, "\thw6 keyexpand -k=key -t=tablefile\n");
		fprintf(stderr, "\thw6 encrypt -k=key -t=tablefile [file]\n");
		fprintf(stderr, "\thw6 decrypt -k=key -t=tablefile [file]\n");
		fprintf(stderr, "\thw6 inverse -p=poly\n");
	} else {
		fprintf(stderr, "ERROR: invalid parameters. Should be the following:\n");
		if(strcmp(type, "tablecheck") == 0) {
			fprintf(stderr, "\thw6 tablecheck -t=tablefile\n");
		} else if(strcmp(type, "modprod") == 0) {
			fprintf(stderr, "\thw6 modprod -p1=poly1 -p2=poly2\n");
		} else if(strcmp(type, "keyexpand") == 0) {
			fprintf(stderr, "\thw6 keyexpand -k=key -t=tablefile\n");
		} else if(strcmp(type, "encrypt") == 0) {
			fprintf(stderr, "\thw6 encrypt -k=key -t=tablefile [file]\n");
		} else if(strcmp(type, "decrypt") == 0) {
			fprintf(stderr, "\thw6 decrypt -k=key -t=tablefile [file]\n");
		} else if(strcmp(type, "inverse") == 0) {
			fprintf(stderr, "\thw6 inverse -p=poly\n");
		}
	}
	return;
}

// Main method of HW6, mostly taking user input and running the correct prog
int main(int argc, char *argv[]) {

	// Look for correct command line arguments
	if(argc <= 1) {
		usage();
		return 0;
	} else {
		if(strcmp(argv[1], "tablecheck") == 0) {
			// Tablecheck prog should be run
			FILE* tablefile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-t=", 3) == 0) {
					tablefile = fopen(argv[i]+3, "r");
					if(tablefile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(tablefile) {
				tablecheck(tablefile);
				fclose(tablefile);
			} else {
				usage("tablecheck");
			}
		}
		else if(strcmp(argv[1], "modprod") == 0) {
			// Modprod prog should be run
			char *poly1 = NULL;
			char *poly2 = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-p1=", 4) == 0) {
					poly1 = argv[i]+4;
				}
				else if(strncmp(argv[i], "-p2=", 4) == 0) {
					poly2 = argv[i]+4;
				}
			}
			if(poly1 && poly2) {
				modprod(poly1, poly2);
			} else {
				usage("modprod");
			}
		}
		else if(strcmp(argv[1], "keyexpand") == 0) {
			// Keyexpand prog should be run
			char *key = NULL;
			FILE* tablefile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-k=", 3) == 0) {
					key = argv[i]+3;
				}
				else if(strncmp(argv[i], "-t=", 3) == 0) {
					tablefile = fopen(argv[i]+3, "r");
					if(tablefile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(key && tablefile) {
				keyexpand(key, tablefile);
				fclose(tablefile);
			} else {
				usage("keyexpand");
			}
		}
		else if(strcmp(argv[1], "encrypt") == 0) {
			// Encrypt prog should be run
			char *key = NULL;
			FILE* tablefile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-k=", 3) == 0) {
					key = argv[i]+3;
				}
				else if(strncmp(argv[i], "-t=", 3) == 0) {
					tablefile = fopen(argv[i]+3, "r");
					if(tablefile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(key && tablefile) {
				if(argv[argc-1][0] != '-') {
					// Take input from file
					FILE* fin = NULL;
					fin = fopen(argv[argc-1], "r");
					if(fin == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-1]);
						return 0;
					}
					encrypt(key, tablefile, fin);
					fclose(fin);
				} else {
					// Take input from cmd line
					encrypt(key, tablefile, stdin);
				}
				fclose(tablefile);
			} else {
				usage("encrypt");
			}
		}
		else if(strcmp(argv[1], "decrypt") == 0) {
			// Decrypt prog should be run
			char *key = NULL;
			FILE* tablefile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-k=", 3) == 0) {
					key = argv[i]+3;
				}
				else if(strncmp(argv[i], "-t=", 3) == 0) {
					tablefile = fopen(argv[i]+3, "r");
					if(tablefile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(key && tablefile) {
				if(argv[argc-1][0] != '-') {
					// Take input from file
					FILE* fin = NULL;
					fin = fopen(argv[argc-1], "r");
					if(fin == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-1]);
						return 0;
					}
					decrypt(key, tablefile, fin);
					fclose(fin);
				} else {
					// Take input from cmd line
					decrypt(key, tablefile, stdin);
				}
				fclose(tablefile);
			} else {
				usage("decrypt");
			}
		}
		else if(strcmp(argv[1], "inverse") == 0) {
			// Inverse prog should be run
			char *poly = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-p=", 3) == 0) {
					poly = argv[i]+3;
				}
			}
			if(poly) {
				inverse(poly);
			} else {
				usage("inverse");
			}
		}
		else {
			// Failed input
			usage();
		}
	}
	
	return 0;
}

