// Kevin Crane
//   CS531 - Homework 7
//   Generating Primes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "primes.h"
#include "trialdiv.h"
#include "millerrabin.h"
#include "rndsearch.h"
#include "maurer.h"


// Called if invalid parameters are initially called
void usage(const char *type="none") {
	if(strcmp(type, "none") == 0) {
		fprintf(stderr, "ERROR: invalid parameters. Should be one of the following:\n");
		fprintf(stderr, "\thw7 primes -n=maxval\n");
		fprintf(stderr, "\thw7 trialdiv -n=number -p=primesfile\n");
		fprintf(stderr, "\thw7 millerrabin -n=number -t=maxitr -p=primesfile\n");
		fprintf(stderr, "\thw7 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n");
		fprintf(stderr, "\thw7 maurer -k=numbits -p=primesfile -r=rndfile\n");
	} else {
		fprintf(stderr, "ERROR: invalid parameters. Should be the following:\n");
		if(strcmp(type, "primes") == 0) {
			fprintf(stderr, "\thw7 primes -n=maxval\n");
		} else if(strcmp(type, "trialdiv") == 0) {
			fprintf(stderr, "\thw7 trialdiv -n=number -p=primesfile\n");
		} else if(strcmp(type, "millerrabin") == 0) {
			fprintf(stderr, "\thw7 millerrabin -n=number -t=maxitr -p=primesfile\n");
		} else if(strcmp(type, "rndsearch") == 0) {
			fprintf(stderr, "\thw7 rndsearch -k=numbits -t=maxitr -p=primesfile -r=rndfile\n");
		} else if(strcmp(type, "maurer") == 0) {
			fprintf(stderr, "\thw7 maurer -k=numbits -p=primesfile -r=rndfile\n");
		}
	}
	return;
}

// Main method of HW7, mostly taking user input and running the correct prog
int main(int argc, char *argv[]) {

	// Look for correct command line arguments
	if(argc <= 1) {
		usage();
		return 0;
	} else {
		if(strcmp(argv[1], "primes") == 0) {
			// Primes prog should be run
			int maxval = -1;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-n=", 3) == 0) {
					maxval = atoi(argv[i]+3);
				}
			}
			if(maxval >= 2 && maxval <= 16777216) {		// Must be between 2 and 2^24, inclusive
				primes(maxval);
			} else {
				usage("primes");
				fprintf(stderr, "\tmaxval must be between 2 and 2^24, inclusive.\n");
			}
		}
		else if(strcmp(argv[1], "trialdiv") == 0) {
			// Trialdiv prog should be run
			char *number = NULL;
			FILE* primesfile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-n=", 3) == 0) {
					number = argv[i]+3;
				}
				else if(strncmp(argv[i], "-p=", 3) == 0) {
					primesfile = fopen(argv[i]+3, "r");
					if(primesfile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(number && primesfile) {
				trialdiv(number, primesfile);
				fclose(primesfile);
			} else {
				usage("trialdiv");
			}
		}
		else if(strcmp(argv[1], "millerrabin") == 0) {
			// Miller-Rabin prog should be run
			char *number = NULL;
			int maxitr = -1;
			FILE* primesfile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-n=", 3) == 0) {
					number = argv[i]+3;
				}
				else if(strncmp(argv[i], "-t=", 3) == 0) {
					maxitr = atoi(argv[i]+3);
				}
				else if(strncmp(argv[i], "-p=", 3) == 0) {
					primesfile = fopen(argv[i]+3, "r");
					if(primesfile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(number && primesfile && maxitr > 0) {
				millerrabin(number, maxitr, primesfile);
				fclose(primesfile);
			} else {
				usage("millerrabin");
			}
		}
		else if(strcmp(argv[1], "rndsearch") == 0) {
			// Random search prog should be run
			int numbits = -1;
			int maxitr = -1;
			FILE* primesfile = NULL;
			FILE* rndfile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-k=", 3) == 0) {
					numbits = atoi(argv[i]+3);
				}
				else if(strncmp(argv[i], "-t=", 3) == 0) {
					maxitr = atoi(argv[i]+3);
				}
				else if(strncmp(argv[i], "-p=", 3) == 0) {
					primesfile = fopen(argv[i]+3, "r");
					if(primesfile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
				else if(strncmp(argv[i], "-r=", 3) == 0) {
					rndfile = fopen(argv[i]+3, "r");
					if(rndfile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(rndfile && primesfile && numbits > 0 && maxitr > 0) {
				rndsearch(numbits, maxitr, primesfile, rndfile);
				fclose(primesfile);
				fclose(rndfile);
			} else {
				usage("rndsearch");
			}
		}
		else if(strcmp(argv[1], "maurer") == 0) {
			// Maurer prog should be run
			int numbits = -1;
			FILE* primesfile = NULL;
			FILE* rndfile = NULL;
			for(int i=2; i<argc; i++) {
				if(strncmp(argv[i], "-k=", 3) == 0) {
					numbits = atoi(argv[i]+3);
				}
				else if(strncmp(argv[i], "-p=", 3) == 0) {
					primesfile = fopen(argv[i]+3, "r");
					if(primesfile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
				else if(strncmp(argv[i], "-r=", 3) == 0) {
					rndfile = fopen(argv[i]+3, "r");
					if(rndfile == NULL) {
						fprintf(stderr, "ERROR: invalid file '%s' for reading.\n", argv[i]+3);
						return 0;
					}
				}
			}
			if(rndfile && primesfile && numbits > 0) {
				maurer(numbits, primesfile, rndfile);
				fclose(primesfile);
				fclose(rndfile);
			} else {
				usage("maurer");
			}
		}
		else {
			// Failed input
			usage();
		}
	}
	
	return 0;
}

