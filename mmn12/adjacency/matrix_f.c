#include "adjacency.h"

int getMatrix(adjmat matrix)
{
    int i, j;
    for(i=0 ; i<N ; i++) {
        for(j=0 ; j<N ; j++) {
            printf("\nEnter value for matrix[%d][%d]:", i, j);
            scanf("\n%d", &matrix[i][j]);
        }  
    }
    return 0;
}

void printMatrix(adjmat matrix)
{ 
    int i, j;
    printf("\n\nTwo Dimensional array Matrix:\n\n");
    printf("\t");
    for(i=0;i<N;i++){
        printf("\t%d", i);/* print index of matrix */   
    }
    printf("\n\t\t");
    for(i=0;i<N;i++){
        printf("__\t");        
    }
    printf("\n");
    for(i=0; i<N; i++) {
        printf("\n");
        printf("%d\t|",i); /* print index of matrix */
        
        for(j=0;j<N;j++) {
            printf("\t%d",matrix[i][j]);
            if(j==N-1) {
                printf("\n");
            }
        }
    }
}
int matrixCheck(adjmat matrix) 
{
    int u, v;
    printf("\nPlease enter row value and col value followed by space: ");

        if(scanf("%d", &u)==EOF)return -1;
        scanf("%d",&v);
        
        if(u == -1 && v == -1) return -1;
        if(path(matrix, u, v)) {
            printf("\n>>>>>>>>>>The requested values %d, and %d have an ancestor (: \n",u,v);
            printf("\nPlease enter row value and col value followed by space: ");
        }else {
            printf("\n>>>>>>>>>>The requested values %d, and %d dont have an ancestor!!\n",u,v);
        }
    printf("\nif you want to try again with differnt values type them or type -1 -1 to stop.\n");
    return matrixCheck(matrix);
}

int path(adjmat matrix, int u, int v) {
    int i;
    if (u>=N || v>=N || u<0 || v<0) {
        return FALSE;
    }
    if ((u==v) || (matrix[u][v]==1)) {
        return TRUE;
    }
    for (i=0; i<N; i++) {
        if (matrix[u][i] != 0) {
            if (path(matrix,i,v)) {
                return TRUE;
            }
        }
    }
    return FALSE;
}