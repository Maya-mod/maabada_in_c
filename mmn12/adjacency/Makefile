adjacency: matrix_f.o adjacency.o
		gcc -g -ansi -Wall -pedantic adjacency.o matrix_f.o -o adjacency 

adjacency.o: adjacency.h adjacency.c
		gcc -c -g -ansi -Wall -pedantic adjacency.c -o adjacency.o

matrix_f.o: adjacency.h matrix_f.c
		gcc -c -g -ansi -Wall -pedantic matrix_f.c -o matrix_f.o