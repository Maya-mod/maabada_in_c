#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*Type for complex number*/
typedef struct complex {float real; float img;} complex;

/* Reads the given arguments into the given complex pointers. */
void read_comp(complex*, float real, float img);

/* Prints the given complex number */
void print_comp(complex*);

/* Adds the complex(s) values and print */
void add_comp(complex*, complex*);

/* Subs the complex(s) values and print */
void sub_comp(complex*, complex*);

/* Multiplies the complex with the real and print */
void mult_comp_real(complex*, float);

/* Multiplies the complex with the imaginary and print */
void mult_comp_img(complex*, float);

/* Multiplies the two complex and print */
void mult_comp_comp(complex*, complex*);

/* Calculate the absolute value of the complex. */
void abs_comp(complex*);

/* Stop the program */
void stop();