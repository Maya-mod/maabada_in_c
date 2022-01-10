assembler: linkedList.o firstpass.o secondpass.o functions.o assembler.o
	gcc -g -ansi -Wall -pedantic -o assembler assembler.o firstpass.o secondpass.o linkedList.o functions.o -lm
assembler.o: assembler.h assembler.c 
	gcc -c -g -ansi -Wall -pedantic assembler.c -o assembler.o
firstpass.o: assembler.h firstpass.c
	gcc -c -g -ansi -Wall -pedantic firstpass.c -o firstpass.o
secondpass.o: assembler.h secondpass.c
	gcc -c -g -ansi -Wall -pedantic secondpass.c -o secondpass.o
linkedList.o: assembler.h linkedList.c
	gcc -c -g -ansi -Wall -pedantic linkedList.c -o linkedList.o
functions.o: functions.h functions.c
	gcc -c -g -ansi -Wall -pedantic functions.c -o functions.o
clean:
	-rm *.o