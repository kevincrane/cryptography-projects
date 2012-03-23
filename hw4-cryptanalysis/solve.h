// Kevin Crane
//   CS531 - Homework 4
//   solve.h

#ifndef SOLVE_H
#define SOLVE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Return true if character is lowercase letter
bool is_letter(char character);

// ##### KASISKI ANALYSIS #####

// Returns true if the word_buf of length len is made up entirely of letters
bool is_only_letters(char* word_buf, int len);

// Run Kasiski analysis on text, looking for matches of length word_len
bool kasiski_analyze(char* text, int file_len, int word_len);

// Analyze the input text file using the Kasiski method
void kasiski(char *text, int file_len);

// ##### INDEX OF CORRELATION #####

// Return a float value with the 'index of coincidence' for this cipher text
float calc_ic(int char_count[], int tot_count);

// Return a float value with the expected 'index of coincidence' for a ciphertext with key of length per
float calc_eic(int per, int tot_count);

// Analyze the input text file using the index of coincidence method
void index_of_coincidence(char *text, int file_len, int maxt);

// ##### AUTO-CORRELATION #####

// Calculate auto-correlation count for text file with given period
void calc_ac(char *text, int file_len, int per);

// Analyze the input text file using the auto-correlation method
void auto_correlation(char *text, int file_len, int maxt);


// ##### MAIN PROGRAM #####

// Produce a histogram of ciphertext character frequencies
void solve(int maxt, FILE *text);

#endif
