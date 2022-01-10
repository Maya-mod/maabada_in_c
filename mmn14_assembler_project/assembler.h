#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

/* Assembler main function prototypes */

void formatFile(char *);
FILE * fopen_f(char *, char *, char *);
extern int firstPass(FILE *, int *, int *, lptr *, dptr *, iptr *);
extern int secondPass(FILE *, int *, int *, lptr *, dptr *, iptr *);
int print_files(lptr *, dptr *, iptr *, char *, int, int);

#endif