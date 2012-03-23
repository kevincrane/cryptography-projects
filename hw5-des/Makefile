# Makefile for CS531 Homework 5


CC = g++
FLAGS = -Wall -g
OUTPUT = hw5

hw5: tablecheck.o encrypt.o decrypt.o encrypt3.o decrypt3.o hw5.c
	$(CC) $(FLAGS) tablecheck.o encrypt.o decrypt.o encrypt3.o decrypt3.o hw5.c -o $(OUTPUT)
	
tablecheck.o: tablecheck.c
	$(CC) $(FLAGS) -c tablecheck.c
	
encrypt.o: encrypt.c
	$(CC) $(FLAGS) -c encrypt.c
	
decrypt.o: decrypt.c
	$(CC) $(FLAGS) -c decrypt.c
	
encrypt3.o: encrypt3.c
	$(CC) $(FLAGS) -c encrypt3.c
	
decrypt3.o: decrypt3.c
	$(CC) $(FLAGS) -c decrypt3.c

	
clean:
	rm -f *.o $(OUTPUT)
