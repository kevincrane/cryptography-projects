# Makefile for CS531 Homework 6


CC = g++
FLAGS = -Wall -g
OUTPUT = hw6

hw6: tablecheck.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o hw6.c
	$(CC) $(FLAGS) tablecheck.o modprod.o keyexpand.o encrypt.o decrypt.o inverse.o hw6.c -o $(OUTPUT)
	
tablecheck.o: tablecheck.c tablecheck.h
	$(CC) $(FLAGS) -c tablecheck.c
	
modprod.o: modprod.c modprod.h
	$(CC) $(FLAGS) -c modprod.c
	
keyexpand.o: keyexpand.c keyexpand.h
	$(CC) $(FLAGS) -c keyexpand.c
	
encrypt.o: encrypt.c encrypt.h
	$(CC) $(FLAGS) -c encrypt.c
	
decrypt.o: decrypt.c decrypt.h
	$(CC) $(FLAGS) -c decrypt.c
	
inverse.o: inverse.c inverse.h
	$(CC) $(FLAGS) -c inverse.c

	
clean:
	rm -f *.o $(OUTPUT)
