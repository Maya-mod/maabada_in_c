#include "assembler.h"

int firstPass(FILE *fp, int *IC, int *DC, lptr *labelPtr, dptr *dataPtr, iptr *instPtr)
{
    int guidanceType = 0, line_num = 0, L, labelSize;
    char line[MAX_LINE], currLabel[MAX_LABEL], *token;
    boolean error_found = FALSE, label_declared = FALSE, run = TRUE;

    /* Defining variables to hold the current lines instruction data */
    opTable *currDirective;
    addressingType srcAdrType;
    char srcArg[MAX_LABEL];
    addressingType destAdrType;
    char destArg[MAX_LABEL];

    
    do
    {
        line_num++;
        label_declared = FALSE;
        L = 0;
        if (fgets(line, MAX_LINE, fp) == NULL)
        {
            run = FALSE;
            break;
        }
        if (line == NULL) /* sanity check */
            break;

        if (*line == ';') /* Skipping empty line */
            continue;

        if (check_empty_line(line)) /* Skipping empty line */
            continue;

        if (!check_legal_commas(line, line_num, &error_found))
            continue;
        token = strtok(line, " \t\n");

        if (*token != '.' && (labelSize = check_label_declaration(token, line_num, &error_found)) > 0)
        {
            strncpy(currLabel, token, labelSize);
            currLabel[labelSize] = '\0';
            label_declared = TRUE;
            token = strtok(NULL, " \t\n");
            if (!token) /* found empty label, skipping to next line. */
            {
                printf("\n|Warning| line:[%d] label:[%s] declared with no instruction", line_num, currLabel);
                continue;
            }
        }

        guidanceType = check_guidance_def(token, line_num, &error_found);

        if (guidanceType)
        {
            if (guidanceType > 2) /* if a label was declared and follows .extern or .entry */
            {
                if (label_declared)
                {
                    printf("\n|Warning| line:[%d] ignoring label:[%s], defined after guidance:[%s]", line_num, currLabel, guidanceCodes[guidanceType]);
                    continue;
                }
                else
                {
                    /* collect label, add to symtable and update extern */
                    if (!(token = strtok(NULL, " \t\n")))
                    {
                        fprintf(stderr, "\n|Error| line:[%d] guidance:[%s] marks no label", line_num, guidanceCodes[guidanceType]);
                        error_found = TRUE;
                        continue;
                    }
                    else
                    {
                        if (strlen(token) <= MAX_LABEL && check_legal_label(token, strlen(token), line_num, &error_found))
                        {
                            strcpy(currLabel, token);
                            if ((token = strtok(NULL, " \t\n")))
                            {
                                error_found = TRUE;
                                fprintf(stderr, "\n|Error| line:[%d] extraenous arguments supplied after label", line_num);
                                continue;
                            }
                            if (guidanceType == 3)
                                add_label(labelPtr, currLabel, 0, EXTERNAL, FALSE, line_num, &error_found);

                            continue;
                        }
                    }
                }
            }
            else
            {   /* found .data, collect arguments and insert into array */
                if (label_declared)
                    add_label(labelPtr, currLabel, *DC, DATA, FALSE, line_num, &error_found);
                if (guidanceType == 1)
                    token = strtok(NULL, " ,\t\n");
                else
                    token = strtok(NULL, "\n");
                if (!token)
                {
                    fprintf(stderr, "\n|Error| line:[%d] guidance supplied with no arguments", line_num);
                    error_found = TRUE;
                    continue;
                }
                switch (guidanceType)
                {
                case 1:
                    collect_data_arguments(token, dataPtr, DC, line_num, &error_found);
                    break;
                case 2:
                    collect_string_arguments(token, dataPtr, DC, line_num, &error_found);   
                    break;
                }
            }
            continue;
        }

        if (label_declared)
        {
            add_label(labelPtr, currLabel, *IC, CODE, FALSE, line_num, &error_found);
        }

        if ((currDirective = check_directive_def(token, line_num, &error_found)))
        {
            if (currDirective->opNum < 6)
            {
                collect_directive_arguments(token, srcArg, &srcAdrType, destArg, &destAdrType, 2, line_num, &error_found);
            }
            else if (currDirective->opNum < 15)
            {
                collect_directive_arguments(token, srcArg, &srcAdrType, destArg, &destAdrType, 1, line_num, &error_found);
                srcAdrType = NONE;
            }
            else
            {
                /* if instruction is stop or rts, terminating data */
                srcAdrType = NONE;
                *srcArg = '\0';
                destAdrType = NONE;
                *destArg = '\0';
                if ((token = strtok(NULL, " \t\n")) != NULL)
                {
                    fprintf(stderr, "\n|Error| line:[%d] unexpected arguments after directive:[%s]", line_num, currDirective->opName);
                    error_found = TRUE;
                }
            }
            if (!check_legal_addresingTypes(currDirective, srcAdrType, destAdrType, line_num, &error_found))
            {
                continue;
            }
            L = insert_instructions(currDirective, srcAdrType, srcArg, destAdrType, destArg, instPtr, IC, line_num);
            *IC += L;
            *IC += 1;
        }
    } while (run);
    if (error_found)
        return 0;
    update_dc_tables(labelPtr, dataPtr, IC);

    return error_found;
}