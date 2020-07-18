/* 
Maya
April 11 2020
MMN11 Q2 

This program checks if a particular pattern matches a given text,
and returns the exact location of the best match

 */
#include <stdio.h>
#include <string.h>
#define MAX_SIZE_STRING 100

int indexMatch(char*, char*);
int findMaxInArray(int[MAX_SIZE_STRING], int);
char subArray[MAX_SIZE_STRING];

int main()
{
    char text[MAX_SIZE_STRING];
    char pattern[MAX_SIZE_STRING];
    
    printf("\n>>>Please enter a Pattern :\n");
    fgets(pattern, sizeof(pattern),stdin);
    printf(">>>Please enter the text for the pattern check :\n");
    fgets(text, sizeof(text),stdin);
    printf(">>>The pattern is: %s",pattern);
    printf(">>>The text is: %s",text);

    if(strlen(pattern)>strlen(text))/* Check if the pattern length is bigger then the test length */
    {
        printf("\n>>>It appears that the pattern is larger then the text, \nPlease try again.\n");
    }
    else
    {
        printf(">>>>>>the location for the best match index is : %d\n\n", indexMatch(text,pattern));
    }
    fflush(stdin);
    return 0;
}
/* Count how many matches there are from every index
and place those sum's in a new array */
int indexMatch(char* text, char* pattern)
{
    int textLength = strlen(text);
	int patternLength = strlen(pattern);
    int subArrayLen = textLength-patternLength+1;
    int subArray[MAX_SIZE_STRING];
    
    int i=0, j=0, textPosition=0, subMatch=0;
    
    for(i=0 ; i < textLength-patternLength+1 ; i++)/* Run all over the text once */
    {
        subMatch = 0; /* Reset the counter */
        textPosition = i;/* Index for internal loop */
        for(j=0 ; j < patternLength-1 ; j++)/* Run from the index of the text until end of pattern */
        {   
            if(text[textPosition] == pattern[j])/* Count if we have a match! */
            {
                subMatch++;
                textPosition++;         
            }
            else
                textPosition++;/* Otherwise, just increase the index */    
        }
        subArray[i] = subMatch; /* Inserts the subMatch values into new array */
    }
    return findMaxInArray(subArray, subArrayLen);
}
/* 
Run over the array once, find the maximum number, 
and return it's index
 */
int findMaxInArray(int subArray[], int sizeArray)
{
    int i=1, j=0;
    int maximum = subArray[0];
    for(i=1 ; i < sizeArray ; i++)
    {
        if(maximum < subArray[i])
        {
            maximum = subArray[i];
            j = i;
        }
    }
    return j;
}
