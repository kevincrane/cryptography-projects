// Kevin Crane
//   CS531 - Homework 7
//   maurer.c

#include <openssl/crypto.h>
#include <openssl/bn.h>

#include "trialdiv.h"
#include "rndsearch.h"
#include "maurer.h"

#define round(X) (((X) >= 0) ? (int)((X)+0.5) : (int)((X)-0.5))


// Reads a single byte from rndfile and returns it
unsigned char RndByte(FILE* rndfile) {
	unsigned char rnd_byte;
	if(fread(&rnd_byte, 1, 1, rndfile) <= 0) {
		fprintf(stderr, "ERROR: Ran out of random bits in rndfile!\n");
		return 0xff;
	}
	return rnd_byte;
}


// Recursive Maurer algorithm for generating a numbits-bit long prime number
BIGNUM* maurer_recurse(int level, int numbits, FILE* primesfile, FILE* rndfile) {
	BIGNUM* prime = BN_new();
	BN_CTX* ctx = BN_CTX_new();
	
	// Print recursive header
	printf("Maurer: level %d, k=%d\n", level, numbits);
	
	// Just do random search if numbits <= 20 long (Step 1)
	while(numbits <= 20) {
		RndOddNum(numbits, prime, rndfile);
		printf("  step 1.1, n = %s\n", BN_bn2dec(prime));
		printf("    ");
		if( trialdiv(BN_bn2dec(prime), primesfile) ) {
			return prime;
		}
	}
	
	// Constants (Steps 2-4)
	int m = 20;
	float r = 0.0;
	
	if(numbits <= 2*m) {
		r = 0.5;
		printf("  step 4, r = %d%%\n", round(r*100));
	} else {
		unsigned char rnd_byte;
		while(numbits > 2*m) {
			rnd_byte = RndByte(rndfile);
			r = 0.5 + ((rnd_byte/255.0) / 2.0);
			if(numbits*(1-r) > m) {
				printf("  step 4: random byte = %d, r = %d%%\n", rnd_byte, round(r*100));
				break;
			}
		}
	}
	
	// Recursion (Step 5)
	BIGNUM* q_val = BN_new();
	q_val = maurer_recurse( level+1, floor(numbits*r)+1, primesfile, rndfile );
	printf("Maurer: back to level %d, k=%d, q=%s\n", level, numbits, BN_bn2dec(q_val));
	
	// New variables and I=(2^(numbits-2))/q_val (Step 6)
	int num_bits_in_q = BN_num_bits(q_val);
	BIGNUM* two = NULL;
	BN_dec2bn(&two, "2");
	BIGNUM* i_val = BN_new();
	BIGNUM* numbits_minus_two = BN_new();
	BN_set_word(numbits_minus_two, numbits-2);
	BIGNUM* exponent = BN_new();
	BN_exp(exponent, two, numbits_minus_two, ctx);
	BN_div(i_val, NULL, exponent, q_val, ctx);
	BN_free(exponent);
	BN_free(numbits_minus_two);

	// BigNum operations and outputting prime (Step 7)
	int count = 1;
	while(true) {
		// Step 7.1
		BIGNUM* r_val = BN_new();
		RndOddNum(numbits+1-num_bits_in_q, r_val, rndfile);
		BN_mod(r_val, r_val, i_val, ctx);
		BN_add(r_val, r_val, i_val);
		BN_add(r_val, r_val, BN_value_one());
		BN_mul(prime, two, r_val, ctx);
		BN_mul(prime, prime, q_val, ctx);
		BN_add(prime, prime, BN_value_one());
		printf("  step 7, itr %d: R = %s, n = %s\n", count, BN_bn2dec(r_val), BN_bn2dec(prime));
		printf("    ");
		
		// Step 7.2
		if(trialdiv(BN_bn2dec(prime), primesfile)) {
			int num_bits_in_prime = BN_num_bits(prime);
			BIGNUM* a_val = BN_new();
			BIGNUM* n_minus_one = BN_new();
			BN_sub(n_minus_one, prime, BN_value_one());
			
			// Step 7.2.1
			do {
				RndOddNum(num_bits_in_prime, a_val, rndfile);
			} while( BN_cmp(a_val, BN_value_one())!=1 || BN_cmp(a_val, n_minus_one)!=-1 );
			printf("  step 7.2.1, itr %d: a = %s\n", count, BN_bn2dec(a_val));
			
			// Step 7.2.2
			BIGNUM* b_val = BN_new();
			BN_mod_exp(b_val, a_val, n_minus_one, prime, ctx);
			if(BN_cmp(b_val, BN_value_one()) == 0) {
				BIGNUM* d_val = BN_new();
				BN_mul(r_val, r_val, two, ctx);
				BN_mod_exp(b_val, a_val, r_val, prime, ctx);
				BN_sub(b_val, b_val, BN_value_one());
				BN_gcd(d_val, b_val, prime, ctx);
				
				// Found an output finally
				if(BN_cmp(d_val, BN_value_one()) == 0) {
					return prime;
				}
			}
		}
		count++;
	}
	
}


// Generate a numbits-bit provable prime using the Maurer algorithm
void maurer(int numbits, FILE* primesfile, FILE* rndfile) {
	
	// Initialize recursive Maurer algorithm
	BIGNUM* prime_num = BN_new();
	prime_num = maurer_recurse(0, numbits, primesfile, rndfile);
	
	// Print final output
	printf("\nMaurer's Algorithm found an %d-bit prime:\n", BN_num_bits(prime_num));
	printf("  n = %s\n", BN_bn2dec(prime_num));
	
	
	// PSEUDO-CODE
	// - Generates a numbit-bit large prime number using the following algorithm:
	// - Maurer(k):
	// 1) if (k <= 20) do forever {
	//    1.1) n = RndOddNum(k)
	//    1.2) if (TrialDivision(n) == "pass") return n
	//    }
	// 2) c = 0.1, m = 20
	// 3) B = c * k^2 (not used)
	// 4) (generate a fraction r, the size of q relative to n)
	//    4.1) if (k <= 2m) r = 0.5
	//    4.2) if (k > 2m) do forever {
	// 	    4.2.1) r = RndByte() / 255.0
	// 	    4.2.2) r = 0.5 + r / 2.0
	// 	    4.2.3) if (k*(1-r) > m) break;
	// 	    }
	// 5) (recursion) q = Maurer(floor(r*k)+1)
	// 6) num_bits_in_q = BN_num_bits(q)
	//    I = floor(2^(k-2) / q)
	// 7) do forever {
	//    7.1) R = RndOddNum(k+1-num_bits_in_q)
	// 	    R = (R mod I) + I + 1
	// 	    n = 2Rq+1
	//    7.2) if (TrialDivision(n) != "fail") {
	// 	           num_bits_in_n = BN_num_bits(n)
	// 	    7.2.1) do {
	// 	               a = RndOddNum(num_bits_in_n)
	// 	           } while (a <= 1 or a >= n-1))
	// 	    7.2.2) b = a^{n-1} mod n
	// 	           if (b == 1) {
	// 	               b = a^{2R} mod n
	// 	               d = gcd(b-1,n)
	// 	               if (d == 1) return(n)
	// 	           }
	// 	    }
	//    }
}
