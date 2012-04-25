// Kevin Crane
//   CS531 - Homework 7
//   primes.c

#include "primes.h"

#define DEBUG false


// Print long unsigned int as bytes
void print_int_bytes(unsigned int val) {
	unsigned char* byte_point = (unsigned char*)&val;
	for(int i=0; i<4; i++)
		printf("%c", byte_point[i]);
}

// Print all primes between 2 and maxval, inclusive
void sieve(unsigned int maxval) {
	bool* primes = (bool*)calloc(maxval+1, sizeof(bool));	// If primes[i] is true, i is a prime
	
	// Initialize primes array
	primes[0] = false;
	primes[1] = false;
	for(unsigned int i=2; i<=maxval; i++) {
		primes[i] = true;
	}
	
	// Run the sieve for each value up to sqrt(maxval)
	for(unsigned int i=2; i<=maxval; i++ ) {
		// Found a prime, push multiples through sieve
		if(primes[i]) {
			for(unsigned int j=i*2; j<=maxval; j+=i) {
				primes[j] = false;
			}
		}
	}
	
	// Print primes that were found
	int count = 0;
	for(unsigned int i=2; i<=maxval; i++) {
		if(primes[i]) {
			if(!DEBUG) {
				print_int_bytes(htonl(i));
			} else {
				printf("%u ", i);
				count++;
			}
		}
	}
	
	if(DEBUG) 
		printf("\nThere are %u primes less than or equal to %u.\n", count, maxval);
	
	free(primes);
}

// Generate all prime numbers less than or equal to maxval. 
//	maxval must be between 2 and 2^24, inclusive.
void primes(unsigned int maxval) {
	
	// Print primesfile header
	if(!DEBUG) {
		print_int_bytes(htonl(maxval));
	} else {
		printf("Finding primes between 2 and %u.\n", maxval);
	}
	
	// Find all prime numbers
	sieve(maxval);
	
	// PSEUDO-CODE
	// * Change DEBUG value to 'true' at top of code to make primes human readable on x86 architecture
	// - Create an array of booleans of size maxval, all defaulted to true
	// - Starting with i=2, change all multiples of i in the array to false
	// - Increment i to maxval, changing multiples of values if i is a prime number according to the array
	// - Once every non-prime number has been pushed through the sieve, output all prime numbers that are remaining
}
