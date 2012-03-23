// Kevin Crane
//   CS531 - Homework 4
//   histo.h

#ifndef HISTO_H
#define HISTO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int ALPHA_LEN = 26;

// Print character counts, formatted and in order
//	NOTE: used slow, selection sort algorithm because list is guaranteed small, 
//	implementation is simple, and it requires no extra space or data structures
void print_in_order(int char_count[ALPHA_LEN], int tot_count);

// Produce a histogram of ciphertext character frequencies
void histo(int per, int index, FILE *text);

#endif
