mycomp: complex.o mycomp.o 
	gcc -g -ansi -Wall -pedantic -o mycomp mycomp.o complex.o -lm

complex.o: complex.h complex.c
	gcc -c -g -ansi -Wall -pedantic complex.c -o complex.o -lm
	
mycomp.o: mycomp.h complex.o mycomp.c
	gcc -c -g -ansi -Wall -pedantic mycomp.c -o mycomp.o -lm
