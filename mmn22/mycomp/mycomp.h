/***************************************************************************************** 
Maya
May 31 2020
MMN22

This program is a complex calculator and it's an interactive program that read input
from stdio in a form of command, analyze the command and execute the requested command.
******************************************************************************************/

#include <string.h>
#include "complex.h"
#define NUM_VARS 6 
#define MAX_INPUT 100 /* Max length of the input */

extern complex A, B, C, D, E, F;

struct comps
{
    char name;
    complex *comp;
}comps[NUM_VARS] = {
    {'A', &A},
    {'B', &B},
    {'C', &C},
    {'D', &D},
    {'E', &E},
    {'F', &F}
};

char funcs[10][15] = { "\0",
                        "abs_comp", 
                        "print_comp",
                        "add_comp", 
                        "sub_comp", 
                        "mult_comp_comp", 
                        "mult_comp_real", 
                        "mult_comp_img", 
                        "read_comp", 
                        "stop" 
                      };
/******************************************************************
Check for Multiple consecutive commas\Missing comma\Illegal comma
if found issue the correspond error, if not, continue.
*******************************************************************/
int check_comma();

/*******************************************************************  
Activate the Method_type according to the args
abs_comp || print_comp) args = 1 & method_type = 1
add_comp || sub_comp ||mult_comp_comp) args = 2 method_type = 2
(mult_comp_real || mult_comp_img) args = 2 method_type=2 
(read_comp) args = 3 method_type = 3
(stop) args = 0 
*******************************************************************/
void call_method();

/*******************************************************************
Check if the param's is valid (check_param_complex, check_param_num)
according to the method_type & args and if not give the corresponding
error's
*******************************************************************/
int check_params();

/*******************************************************************
 Check if the command is correct, the option's are - 
 abs_comp, print_comp, add_comp, sub_comp, mult_comp_comp, 
 mult_comp_real, mult_comp_img, read_comp, stop
********************************************************************/
int check_command();

/*******************************************************************
Check if the complex parameter is ether A, B, C, D, E, F.
and if not issue the corresponding error
********************************************************************/
int check_param_complex();

/*******************************************************************
Check if number or not & also check if the number is located in
the right location and if not issue the corresponding error
********************************************************************/
int check_param_num();