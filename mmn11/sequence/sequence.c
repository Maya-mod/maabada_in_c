/* 
Maya
April 11 2020
MMN11 Q1

This program recieve a set of characters and decides if it's 
in rises, realy rises, decline, real decline, or dangling order

 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define _Bool int
#define TRUE 1
#define FALSE 0

void f_sequence(char str[]);


 
int main() {
    char string[100];
    printf("\n>>> Please enter a string:\n");
    while (scanf("%str \n",string) != EOF){}
    
    if(strlen(string)<=100){
        f_sequence(string);   
    }
    else printf("\v>>> The string length is = %ld, it's must be less then 100 char.\n ", strlen(string));
    fflush(stdin);
    return 0;
} 

void f_sequence(char string[]) {
    _Bool reallyUp = TRUE;
    _Bool up = TRUE;
    _Bool reallyDown = TRUE;
    _Bool down = TRUE;

    int i;
    for(i = 0; i<strlen(string)-1; i++) {
        
        if((int)string[i] == (int)string[i+1]){
            reallyUp = FALSE;
            reallyDown = FALSE;
        }
        else if((int)string[i] > (int)string[i+1]) {
            reallyUp = FALSE;
            up = FALSE;
        }
        else if((int)string[i] < (int)string[i+1]){
            reallyDown = FALSE;
            down = FALSE;
        }
        else {
            up = FALSE;
            reallyDown = FALSE;
            down = FALSE;
            reallyUp = FALSE;
        }
    }
 
    if(reallyUp){
        printf(">>> The given string %s is in realy rises\n",string);
    }else if(reallyDown){
        printf(">>> The string %s is in real decline\n",string);
    }else if(up){
        printf(">>> The string %s is rises\n",string);
    }else if(down){
        printf(">>> The string %s is decline\n",string);
    }else{
        printf(">>> The string %s is dangling\n",string);
    }
}