/******************************************************************************************************* 
Maya
June 9 2020
MMN23

This program convers numbers to words (number to string) from 0-99
for argc=1 (./numbers) the input is from the stdio and the output also printed to the stdio
for argc=2 (./numbers input_file) the input is from file and the output printed to the stdio
for argc=3 (./numbers input_file out_put_file) the input is from file the output also printed to a file

the program also handle cases of errors where : permission, file not existed, too many arguments.
********************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* The first string is not used, it is to make array indexing simple */
char *single_digits[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
/* The first string is not used, it is to make array indexing simple */
char *two_digits[] = {"ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"};
/* The first two string are not used, they are to make array indexing simple*/
char *tens_multiple[] = {"", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"};
/* convert the numbers from the stdin or file */
int convert_them_numbers(int, FILE *);
/* read the input from the stdio and call the "convert_them_numbers" function */
void read_input(char *, FILE *);