// Kevin Crane
//   CS531 - Homework 7
//   trialdiv.c

#include <openssl/crypto.h>
#include <openssl/bn.h>

#include "trialdiv.h"


// Returns true if the character string is all digits
bool is_number(char* string) {
	for(unsigned int i=0; i<strlen(string); i++) {
		if(string[i] < '0' || string[i] > '9') {
			return false;
		}
	}
	return true;
}

// Returns true if num%factor == 0, using BigNum
bool is_factor(BIGNUM* num, unsigned int factor, BN_CTX* ctx) {
	BIGNUM* fac = NULL;
	BIGNUM* rem = BN_new();	
	char* factor_text = (char*)calloc(1, 8);
	sprintf(factor_text, "%u", factor);
	BN_dec2bn(&fac, factor_text);
	
	// Calculate modulus remainder
	BN_mod(rem, num, fac, ctx);
	
	// If remainder is 0, factor found
	if(BN_is_zero(rem) && BN_cmp(num, fac) != 0) {
		BN_free(rem);
		return true;
	}

	BN_free(fac);
	free(factor_text);
	BN_free(rem);
	
	return false;
}


// Test the primality of number using trial division by trying sequentially all small prime numbers
bool trialdiv(char* number, FILE* primesfile) {
	unsigned int* factor = (unsigned int*)calloc(sizeof(unsigned int), 1);
	BIGNUM* num = NULL;
	BIGNUM* maxval = NULL;
	BN_CTX* ctx = BN_CTX_new();
	BN_dec2bn(&num, number);
	
	// Verify number is actually number
	if(!is_number(number)) {
		fprintf(stderr, "ERROR: character string for number needs to be all digits.\n");
		return false;
	}
	
	// Verify that maxval is big enough to provide enough primes
	fseek(primesfile, 0, SEEK_SET);
	fread(factor, sizeof(unsigned int), 1, primesfile);
	char* maxval_text = (char*)calloc(1, 8);
	sprintf(maxval_text, "%u", htonl(*factor)+1);
	
	// Iterate through every prime in primesfile, see if it's a factor of number
	while(fread(factor, sizeof(unsigned int), 1, primesfile) > 0) {
		if(is_factor(num, ntohl(*factor), ctx)) {
			printf("n is composite by trial division (mod %u = 0)\n", htonl(*factor));
			BN_free(num);
			BN_CTX_free(ctx);
			free(maxval_text);
			return false;
		}
	}
	
	
	// Check if there's enough primes in primesfile
	BN_dec2bn(&maxval, maxval_text);
	BN_sqr(maxval, maxval, ctx);
	if(BN_cmp(maxval, num) <= 0) {
		// Not enough values in primesfile
		printf("n passes trial division test (not enough primes)\n");
	} else {
		// Value is prime
		printf("n passes trial division test\n");
	}
	
	BN_free(maxval);
	BN_free(num);
	BN_CTX_free(ctx);
	free(maxval_text);
	
	return true;
	
	
	// PSEUDO-CODE
	// - Reads prime numbers one at a time from a binary file, primesfile
	// - Divides the number to be tested by each prime number
	//   - If the remainder is 0 and the number != the prime, return false (it's a composite)
	// - If by the end of the primesfile, nothing evenly divides into it:
	//   - If the number is larger than the square of the maxval of the primesfile, return true (not enough primes)
	//   - Otherwise, return true (it's a prime)
}
