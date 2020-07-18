#include "adjacency.h"

int main()
{   
    adjmat matrix;
    printf("The matrix size is: %d\n", N);
    getMatrix(matrix);
    printMatrix(matrix);
    matrixCheck(matrix);
    fflush(stdin);
    return 0;
}