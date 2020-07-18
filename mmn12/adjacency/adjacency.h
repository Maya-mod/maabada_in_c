/***************************************************************************************** 
Maya
April 30 2020
MMN12 Q1

This program builds two dimension matrix by a fixed size
And asks the user to enter 0 or 1 data to fill the matrix to build a neighboring tree
Then ask from the user to enter location specified by row and col (u & v) values
to determine if the given index has an ancestor
 *****************************************************************************************/

#include <stdio.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define MAX_SIZE 100
enum size {N=20};
typedef int adjmat[N][N];

/* Request values from user to fill the matrix */
int getMatrix(adjmat matrix);
/* Print the matrix */
void printMatrix(adjmat);
/* Recursive check for the given value to find if there is an ancestor */
int matrixCheck(adjmat matrix);
/* Check the neighbors */
int path(adjmat, int u, int v);