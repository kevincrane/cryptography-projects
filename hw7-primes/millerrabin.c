// Kevin Crane
//   CS531 - Homework 7
//   millerrabin.c

#include <openssl/crypto.h>
#include <openssl/bn.h>

#include "millerrabin.h"


// Find R and S values such that (2^s)*r == n-1
void find_r_s(BIGNUM* n_minus_one, BIGNUM* r, BIGNUM* s, BN_CTX* ctx) {
	BIGNUM* two = NULL;
	BN_dec2bn(&two, "2");
	BN_zero(s);
	BN_copy(r, n_minus_one);
	while(!BN_is_odd(r)) {
		BN_add(s, s, BN_value_one());
		BN_div(r, NULL, r, two, ctx);
	}
	
	BN_free(two);
}

// Test the primality of number using the Miller-Rabin algorithm
bool millerrabin(char* number, int maxitr, FILE* primesfile, const char* lead_space) {
	
	unsigned int* prime = (unsigned int*)calloc(sizeof(unsigned int), 1);
	fseek(primesfile, sizeof(unsigned int), SEEK_SET);
	
	// Verify number is actually number
	if(!is_number(number)) {
		fprintf(stderr, "ERROR: character string for number needs to be all digits.\n");
		return false;
	}
	
	// Initialize BigNums
	BIGNUM* num = NULL;
	BIGNUM* a_val = BN_new();
	BIGNUM* y_val = BN_new();
	BIGNUM* r = BN_new();
	BIGNUM* s = BN_new();
	BIGNUM* n_minus_one = BN_new();
	BN_CTX* ctx = BN_CTX_new();
	BN_dec2bn(&num, number);
	BN_sub(n_minus_one, num, BN_value_one());
	
	// Calculate r and s values
	find_r_s(n_minus_one, r, s, ctx);	
	int s_int = atoi( BN_bn2dec(s) );
	
	// Print variable states
	if(strcmp(lead_space, "") == 0)			// Skip the first line if calling from Random Search Algorithm
		printf("%sn = %s\n", lead_space, BN_bn2dec(num));
	printf("%s  n-1 = %s\n", lead_space, BN_bn2dec(n_minus_one));
	printf("%s  s = %s\n", lead_space, BN_bn2dec(s));
	printf("%s  r = %s\n", lead_space, BN_bn2dec(r));
	
	// Run primality test for up to maxitr iterations
	for (int i=1; i<=maxitr; i++) {
		if( fread(prime, sizeof(unsigned int), 1, primesfile) != 1 ) {
			fprintf(stderr, "ERROR: not enough primes in primesfile.\n");
			return false;
		}
		
		BN_set_word(a_val, ntohl(*prime));
		if(BN_cmp(a_val, n_minus_one) == 1) {
			fprintf(stderr, "ERROR: prime number %s is larger than number testing.\n", BN_bn2dec(a_val));
			return false;
		}
		
		// Compute y=a^r mod n
		BN_mod_exp(y_val, a_val, r, num, ctx);
		if(BN_cmp(y_val, n_minus_one) == 0)
			printf("%s  Itr %d of %d, a = %s, y = %s (which is n-1)\n", lead_space, i, maxitr, BN_bn2dec(a_val), BN_bn2dec(y_val));
		else
			printf("%s  Itr %d of %d, a = %s, y = %s\n", lead_space, i, maxitr, BN_bn2dec(a_val), BN_bn2dec(y_val));
		
		if( BN_cmp(y_val, BN_value_one()) != 0 && BN_cmp(y_val, n_minus_one) != 0 ) {
			// Iterate through a few times
			for(int j=1; (j<=s_int-1) && (BN_cmp(y_val, n_minus_one) != 0); j++) {
				// Compute y=y^2 mod n
				BN_mod_sqr(y_val, y_val, num, ctx);
				
				if(BN_cmp(y_val, n_minus_one) == 0)
					printf("%s    j = %d of %d, y = %s (which is n-1)\n", lead_space, j, s_int-1, BN_bn2dec(y_val));
				else
					printf("%s    j = %d of %d, y = %s\n", lead_space, j, s_int-1, BN_bn2dec(y_val));
				
				// If y == 1, num is composite
				if(BN_cmp(y_val, BN_value_one()) == 0) {
					printf("%sMiller-Rabin found a strong witness %s\n", lead_space, BN_bn2dec(a_val));
					return false;
				}
			}
			
			// If y != (n-1), num is composite
			if(BN_cmp(y_val, n_minus_one) != 0) {
				printf("%sMiller-Rabin found a strong witness %s\n", lead_space, BN_bn2dec(a_val));
				return false;
			}
		}
	}
	
	// Print success message
	printf("%sMiller-Rabin declares n to be a prime number\n", lead_space);
	
	BN_free(num);
	BN_free(a_val);
	BN_free(y_val);
	BN_free(r);
	BN_free(s);
	BN_free(n_minus_one);
	BN_CTX_free(ctx);
	
	return true;	
		
	
	// PSEUDO-CODE
	// - Tests the primality of a number using the following algorithm:
	// - Miller-Rabin(n,maxitr):
	// 1) write n-1 = 2sr such that r is odd
	// 2) for (i=1; i <= maxitr; i++) {
	//    2.1) a = the ith smallest prime number
	// 	    2.1.1) if (a > n-1) return("failure")
	//    2.2) compute y = ar mod n
	//    2.3) if (y != 1 and y != n-1) {
	// 	    2.3.1) for (j=1; j <= s-1 and y != n-1; j++) {
	// 	           2.3.1.1) y = y2 mod n
	// 	           2.3.1.2) if (y == 1) return("composite")
	// 	           }
	// 	    2.3.2) if (y != n-1) return("composite")
	// 	    }
	//    }
	// 3) return("prime")
}
