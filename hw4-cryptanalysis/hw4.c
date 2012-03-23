// Kevin Crane
//   CS531 - Homework 4
//   Cryptanalysis

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keygen.h"
#include "crypt.h"
#include "invkey.h"
#include "histo.h"
#include "solve.h"


// Called if invalid parameters are initially called
void usage(const char *type="none") {
	if(strcmp(type, "none") == 0) {
		fprintf(stderr, "ERROR: invalid parameters. Should be one of the following:\n");
		fprintf(stderr, "\thw4 keygen -t=period\n");
		fprintf(stderr, "\thw4 crypt -k=keyfile [file]\n");
		fprintf(stderr, "\thw4 invkey keyfile\n");
		fprintf(stderr, "\thw4 histo -t=period -i=which [file]\n");
		fprintf(stderr, "\thw4 solve -l=max_t file\n");
	} else {
		fprintf(stderr, "ERROR: invalid parameters. Should be the following:\n");
		if(strcmp(type, "keygen") == 0) {
			fprintf(stderr, "\thw4 keygen -t=period\n");
		} else if(strcmp(type, "crypt") == 0) {
			fprintf(stderr, "\thw4 crypt -k=keyfile [file]\n");
		} else if(strcmp(type, "invkey") == 0) {
			fprintf(stderr, "\thw4 invkey keyfile\n");
		} else if(strcmp(type, "histo") == 0) {
			fprintf(stderr, "\thw4 histo -t=period -i=which [file]\n");
		} else if(strcmp(type, "solve") == 0) {
			fprintf(stderr, "\thw4 solve -l=max_t file\n");
		}
	}
	return;
}

// Main method of HW4, mostly taking user input and running the correct prog
int main(int argc, char *argv[]) {

	// Look for correct command line arguments
	if(argc <= 1) {
		usage();
		return 0;
	} else {
		if(strcmp(argv[1], "keygen") == 0) {
			// Keygen prog should be run
			int period = -1;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-t=", 3) == 0) {
					period = atoi(argv[i]+3);
				}
			}
			if(period > 0) {
				keygen(period);
			} else {
				usage("keygen");
			}
		}
		else if(strcmp(argv[1], "crypt") == 0) {
			// Crypt prog should be run
			FILE* keyfile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-k=", 3) == 0) {
					keyfile = fopen(argv[i]+3, "r");
					if(keyfile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(keyfile) {
				if(argv[argc-1][0] != '-') {
					// Take input from file
					FILE* fin = NULL;
					fin = fopen(argv[argc-1], "r");
					if(fin == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-1]);
						return 0;
					}
					crypt(keyfile, fin);
					fclose(fin);
				} else {
					// Take input from cmd line
					crypt(keyfile, stdin);
				}
				fclose(keyfile);
			} else {
				usage("crypt");
			}
		}
		else if(strcmp(argv[1], "invkey") == 0) {
			// Invkey prog should run
			if(argc != 3) {
				usage("invkey");
				return 0;
			} else {
				// Take input from file
				FILE *fin = NULL;
				fin = fopen(argv[argc-1], "r");
				if(fin == NULL) {
					fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-1]);
					return 0;
				}
				invkey(fin);
				fclose(fin);
			}
		}
		else if(strcmp(argv[1], "histo") == 0) {
			// Histo prog should run
			int period = -1;
			int which = -1;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-t=", 3) == 0) {
					period = atoi(argv[i]+3);
				} else if(strncmp(argv[i], "-i=", 3) == 0) {
					which = atoi(argv[i]+3);
				}
			}
			if(period > 0 && which > 0) {
				if(argv[argc-1][0] != '-') {
					// Take input from file
					FILE *fin = NULL;
					fin = fopen(argv[argc-1], "r");
					if(fin == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-1]);
						return 0;
					}
					histo(period, which, fin);
					fclose(fin);
				} else {
					// Take input from cmd line
					histo(period, which, stdin);
				}
			} else {
				usage("histo");
			}
		}
		else if(strcmp(argv[1], "solve") == 0) {
			// Crypt prog should be run
			int maxt = -1;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-l=", 3) == 0) {
					maxt = atoi(argv[i]+3);
				}
			}
			if(maxt > 0) {
				// Take input from file
				FILE *fin = NULL;
				fin = fopen(argv[argc-1], "r");
				if(fin == NULL) {
					fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[argc-1]);
					return 0;
				}
				solve(maxt, fin);
				fclose(fin);
			} else {
				usage("solve");
			}
		}
		else {
			// Failed input
			usage();
		}
	}
	
	return 0;
}

