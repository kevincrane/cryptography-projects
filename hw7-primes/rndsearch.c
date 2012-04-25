// Kevin Crane
//   CS531 - Homework 7
//   rndsearch.c

#include <openssl/crypto.h>
#include <openssl/bn.h>

#include "rndsearch.h"
#include "trialdiv.h"
#include "millerrabin.h"


// Takes in a BigNum, sets bits 0 and k-1 to 1, bits >= k to 0
void RndOddNum(int numbits, BIGNUM* odd_num, FILE* rndfile) {
	int num_bytes = ceil(numbits/8.0);
	unsigned char* num_read = (unsigned char*)calloc(1, num_bytes);
	if(fread(num_read, 1, num_bytes, rndfile) <= 0) {
		fprintf(stderr, "ERROR: Ran out of random bits in rndfile!\n");
		return;
	}
	
	BN_bin2bn(num_read, num_bytes, odd_num);
//	printf("BEFORE=%s\n", BN_bn2dec(odd_num));
	BN_set_bit(odd_num, 0);
	BN_set_bit(odd_num, numbits-1);
	for(int i=numbits; i<num_bytes*8; i++) {
		BN_clear_bit(odd_num, i);
	}
//	printf("AFTER=%s\n", BN_bn2dec(odd_num));
	
	free(num_read);
}


// Generate a numbits-bit probable prime using the Random-Search algorithm 
void rndsearch(int numbits, int maxitr, FILE* primesfile, FILE* rndfile) {
	bool found_prime = false;
	int count = 1;
	
	int num_bytes = ceil(numbits/8.0);
	BIGNUM* num_orig = BN_new();
	char* num_trimmed = (char*)calloc(1, num_bytes);
	
	// Iterate through potential numbers in RandomFile
	while(!found_prime) {
		printf("RANDOM-SEARCH: iteration %d\n", count);
		
		// Generate random odd numbit-long integer
		RndOddNum(numbits, num_orig, rndfile);
		num_trimmed = BN_bn2dec(num_orig);
		
		// Test with trial division
		printf("  n = %s\n", num_trimmed);
		printf("  ");
		if(trialdiv(num_trimmed, primesfile)) {
			// Passed trial division, check Miller-Rabin
			if(millerrabin(num_trimmed, maxitr, primesfile, "  ")) {
				found_prime = true;
			}
		}
		
		count++;
	}
	
	free(num_trimmed);
	
	// PSEUDO-CODE
	// - 
}
