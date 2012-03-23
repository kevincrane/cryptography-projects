# Makefile for CS531 Homework 1


CC = g++
FLAGS = -Wall
OUTPUT = hw1

hw1: hexdump.o enc_base64.o dec_base64.o hw1.c
	$(CC) $(FLAGS) hw1.c hexdump.o enc_base64.o dec_base64.o -o hw1
	
hexdump.o: hexdump.c
	$(CC) $(FLAGS) -c hexdump.c
	
enc_base64.o: enc_base64.c
	$(CC) $(FLAGS) -c enc_base64.c
	
dec_base64.o: dec_base64.c
	$(CC) $(FLAGS) -c dec_base64.c
	
	
clean:
	rm -f *.o *.hex *.dat *.b64 $(OUTPUT)
