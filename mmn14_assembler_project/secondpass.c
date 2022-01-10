#include "assembler.h"

int secondPass(FILE *fp, int *IC, int *DC, lptr *labelPtr, dptr *dataPtr, iptr *instPtr)
{
    boolean error_found = FALSE, run = TRUE;
    char line[MAX_LINE], *token;
    int line_num = 0;
    int addressToComplete;
    int relativeDistance;
    boolean isExtern = FALSE;
    opTable *currDirective;
    addressingType srcAdrType;
    char srcArg[MAX_LABEL];
    addressingType destAdrType;
    char destArg[MAX_LABEL];

    
    do
    {
        line_num++;

        if ((fgets(line, MAX_LINE, fp)) == NULL)
        {
            run = FALSE;
            break;
        }

        if (line == NULL) /* sanity check */
            break;

        if (check_empty_line(line) || *line == ';')         /* Skipping empty line and comment lines */
        {
            continue;
        }

        token = strtok(line, " \t\n");
        if (token[strlen(token) - 1] == ':')
            token = strtok(NULL, " \t\n");      /* skipping labels declared if present */
        if(!token) continue;
        if (token && *token == '.')
        {
            if (check_guidance_def(token, line_num, &error_found) != 4)
                continue;
            else
            {
                token = strtok(NULL, " \t\n");
                mark_entry_label(labelPtr, token, line_num, &error_found);
            }
            continue;
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
            if (srcAdrType == DIRECT)
            {
                addressToComplete = find_label_address(labelPtr, srcArg, &isExtern, line_num, &error_found);
                if(addressToComplete == -1) continue;
                update_instruction(instPtr, line_num, addressToComplete, isExtern, srcArg);
            }
            if (destAdrType == DIRECT || destAdrType == RELATIVE)
            {
                addressToComplete = find_label_address(labelPtr, destArg, &isExtern, line_num, &error_found);
                if(addressToComplete == -1) continue;
                if(destAdrType == DIRECT)
                {
                    update_instruction(instPtr, line_num, addressToComplete, isExtern, destArg);
                }
                else if(destAdrType == RELATIVE)
                {
                    isExtern = NONE;
                    relativeDistance = find_relative_distance(instPtr, addressToComplete, line_num);
                    update_instruction(instPtr, line_num, relativeDistance, isExtern, destArg);
                }
            }
        }        
    } while (run);
    return error_found;
}