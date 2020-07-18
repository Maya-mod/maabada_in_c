#include "mycomp.h"
char command[15];
char *token, line[MAX_INPUT], line_copy[MAX_INPUT];
int args, method_type, run, first_number, first_complex;
complex A, B, C, D, E, F;
complex *temp_comp_a, *temp_comp_b;
float num_a, num_b;
static int cnt_comma;

int main()
{
    read_comp(&A, 0, 0);
    read_comp(&B, 0, 0);
    read_comp(&C, 0, 0);
    read_comp(&D, 0, 0);
    read_comp(&E, 0, 0);
    read_comp(&F, 0, 0);
    run = 1;
    printf("############################################################################################################");
    printf("\n\t\t                   ~Welcome to my complex calculator~                   \n");
    printf("\n\t\t    Please enter function name followed by A-F letters & two numbers   ");
    printf("\n\t\t                 The availble functions are as followed                      ");
    printf("\n*read_comp* *abs_comp* *print_comp* *add_comp* *sub_comp* *mult_comp_comp* *mult_comp_real* *mult_comp_img*\n");
    printf("\n\t\t                       to stop, just write 'stop'                        \n");
    /* Process inputs until recieving "stop" or "EOF" */
    while (method_type <= 9)
    {
        args = 0;
        method_type = 0;
        first_complex = 0;
        first_number = 0;
        cnt_comma=0;
        printf("\n-----➜");/* prompt */
        if (fgets(line, MAX_INPUT, stdin) == NULL) {
            printf("\nError reached EOF !\n");
            return 0;
        }
        printf("\nThe command you have entered is: ➜ %s \n", line);
        if (check_command()) {
            if (!check_comma()) {
                if (check_params()) {
                    if ((token = strtok(NULL, " \n\t")) == NULL) {
                        call_method();
                    }else if (((token = strtok(NULL, " \n\t")) == NULL)&&((!(token = ",")))) {
                        call_method();                    
                    }
                    else {
                        printf("Extraneous text after end of command\n");   
                    }
                      
                }   
            }      
            
        }
    }
    fflush(NULL);
    return 0;
}

void call_method()
{
    if (!(args == 1 && cnt_comma)) {
        switch (method_type)
        {
        case 1:
            abs_comp(temp_comp_a);
            break;
        case 2:
            print_comp(temp_comp_a);
            break;
        case 3:
            add_comp(temp_comp_a, temp_comp_b);
            break;
        case 4:
            sub_comp(temp_comp_a, temp_comp_b);
            break;
        case 5:
            mult_comp_comp(temp_comp_a, temp_comp_b);
            break;
        case 6:
            mult_comp_real(temp_comp_a, num_a);
            break;
        case 7:
            mult_comp_img(temp_comp_a, num_a);
            break;
        case 8:
            read_comp(temp_comp_a, num_a, num_b);
            break;
        case 9:
            stop();
            break;
        default:
            break;
        }
    }else{
        printf("Extraneous text after end of command\n");
    }
}

int check_params()
{
    token = strtok(line, " \n\t");
    switch (method_type)
    {
    case 1:
    case 2:
        return check_param_complex();
    case 3:
    case 4:
    case 5:
        return check_param_complex() && check_param_complex();
    case 6:
    case 7:
        return check_param_complex() && check_param_num();
    case 8:
        return check_param_complex() && check_param_num() && check_param_num();
    }
    return 0;
}

int check_command()
{
    int i = 1;
    strcpy(line_copy, line);
    token = strtok(line_copy, " \t\n");
    if (token == NULL) {
        return 0;
    }
    for (; i < 10; i++)
    {
        if (!strncmp(token, funcs[i], strlen(funcs[i]))) {
            method_type = i;
            switch (i)
            {
            case 1:
            case 2:
                args = 1;
                return 1;
            case 3:
            case 4:
            case 5:
                args = 2;
                return 1;
            case 6:
            case 7:
                args = 2;
                return 1;
            case 8:
                args = 3;
                return 1;
            case 9:
                if (strlen(token) != strlen(funcs[i]) || (token = strtok(NULL, " \n\t")) != NULL) {
                    printf("Extraneous text after end of command\n");
                    args = 0;
                    return 0;
                }else {
                    stop();
                }
            }
        }
    }
    printf("Undefined command name\n");
    return 1;
}

int check_comma()
{
    int i, comma,tmp_args=args, count = 0;
    if (token == NULL) {
        return 1;
    }
    /* check for Illegal comma  */
    if ((token[0] == ',' || token[strlen(token) - 1] == ',') || (((token = strtok(NULL, " \n\t")) != NULL && token[0] == ','))) {
        printf("Illegal comma\n");
        return 1;
    }
    /* check for Multiple consecutive commas  */
    for (i = 0; i < strlen(line); i++)
    {
        if (line[i] == ',') {
            cnt_comma++;
            if (!count) {
                count = 1;
            }else {
                count = 0;
                printf("Multiple consecutive commas\n");
                return 1;
            }
        }
        else if (line[i] != ' ' && line[i] != '\t') {
            count = 0;
        }
    }
    /* check for Missing comma  */
    if (token == NULL) {
        return 0;
    }
    comma = (token[strlen(token) - 1] == ',') ? 1 : 0;
    if (!comma)
        args--;
    while ((token = strtok(NULL, " \n\t")) != NULL && args)
    {
        if(!comma){
            args--;
        }
        if (!comma && (token[0] != ',') && !args) {
            printf("Missing comma\n");
            return 1;
        }
        comma = (token[strlen(token) - 1] == ',') ? 1 : 0;
    }
    if (args>3||cnt_comma>tmp_args) {
        return 1;
    }
    return 0;
}

int check_param_complex()
{
    int i = 0;
    if ((token = strtok(NULL, " ,\n\t")) == NULL) {
        printf("Missing parameter\n");
        return 0;
    }
    if (strlen(token) > 1){
        printf("Undefined complex variable\n");
        return 0;
    }

    for (; i < NUM_VARS; i++)
    {
        if (token[0] == 'A' + i) {
            if (!first_complex) {
                temp_comp_a = comps[i].comp;
                first_complex = 1;
            }else {
                temp_comp_b = comps[i].comp;
                first_complex = 0;
            }
            return 1;
        }
    }
    printf("Undefined complex variable\n");
    return 0;
}

int check_param_num()
{
    double number;
    char *buffer;
    if ((token = strtok(NULL, " ,\n\t")) == NULL) {
        printf("Missing parameter\n");
        return 0;
    }
    number = strtod(token, &buffer);
    if (strlen(buffer) > 0) {
        printf("Invalid parameter - not a number\n");
        return 0;
    }
    if (!first_number) {
        num_a = number;
        first_number = 1;
    }
    else {
        num_b = number;
        first_number = 0;
    }
    return 1;
}