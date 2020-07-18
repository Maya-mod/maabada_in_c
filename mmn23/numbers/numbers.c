#include "numbers.h"

int main(int argc, char *argv[])
{
    int temp_num;
    FILE *fp2_out;
    char *file_input, *file_output;
    if (argc > 3) {
        fprintf(stderr, "\nToo many files, please try again with 1 or 2 files! \n");
        exit(0);
    }
    switch (argc)
    {
    case 1:
        printf("\nPlease enter number follow by space or enter,\nto stop press ctrl+D\n");
        fp2_out = stdout;
        while ((scanf("\n %d", &temp_num)) != EOF) {
            convert_them_numbers(temp_num, fp2_out);
        }
        break;
    case 2:
        file_input = argv[1];
        fp2_out = stdout;
        read_input(file_input, fp2_out);
        break;
    case 3:
        file_input = argv[1];
        file_output = argv[2];
        if (((fp2_out = fopen(file_output, "w")) == NULL)) {
            fprintf(stderr, "\nThe output file is not accessable!\n"); /* check for access to the file! */
            exit(0);
        }
        read_input(file_input, fp2_out);
        fprintf(fp2_out, "\n");
        fclose(fp2_out);
        break;
    }
    fflush(stdin);
    return 0;
}

void read_input(char *input_dir, FILE *fp_out)
{
    FILE *fp;
    int input;
    if ((fp = fopen(input_dir, "r")) == NULL) {
        fprintf(stderr, "\nThe input file is not accessable! \n"); /* check if the file exist!! */
        exit(0);
    }
    while (fscanf(fp, "%d", &input) != EOF) {
        convert_them_numbers(input, fp_out);
    }
    fclose(fp);
}

int convert_them_numbers(int num, FILE *out)
{
    int tens_temp = (num / 10);
    int single_temp = (num % 10);

    if (tens_temp <= 9 && tens_temp > 1) { /* print from 20-99 */
        if (single_temp != 0) {
            fprintf(out, "%s %s\n", tens_multiple[tens_temp], single_digits[single_temp]);
        }else {
            fprintf(out, "%s\n", tens_multiple[tens_temp]); /* for 20, 30, ...., 90. */
        }
    }
    if (tens_temp == 1) { /* print from 10-19 */
        fprintf(out, "%s\n", two_digits[single_temp]);
    }
    if (tens_temp == 0) { /* print from 0-9 */
        fprintf(out, "%s\n", single_digits[single_temp]);
    }
    return 0;
}