#include "functions.h"

opTable opCodes[OP_CODES_MAX] = {{"mov", 0, 0, MOV, {1, 1, 0, 1}, {0, 1, 0, 1}},
                                 {"cmp", 1, 0, CMP, {1, 1, 0, 1}, {1, 1, 0, 1}},
                                 {"add", 2, 1, ADD, {1, 1, 0, 1}, {0, 1, 0, 1}},
                                 {"sub", 2, 2, SUB, {1, 1, 0, 1}, {0, 1, 0, 1}},
                                 {"lea", 4, 0, LEA, {0, 1, 0, 0}, {0, 1, 0, 1}},
                                 {"clr", 5, 1, CLR, {0, 0, 0, 0}, {0, 1, 0, 1}},
                                 {"not", 5, 2, NOT, {0, 0, 0, 0}, {0, 1, 0, 1}},
                                 {"inc", 5, 3, INC, {0, 0, 0, 0}, {0, 1, 0, 1}},
                                 {"dec", 5, 4, DEC, {0, 0, 0, 0}, {0, 1, 0, 1}},
                                 {"jmp", 9, 1, JMP, {0, 0, 0, 0}, {0, 1, 1, 0}},
                                 {"bne", 9, 2, BNE, {0, 0, 0, 0}, {0, 1, 1, 0}},
                                 {"jsr", 9, 3, JSR, {0, 0, 0, 0}, {0, 1, 1, 0}},
                                 {"red", 12, 0, RED, {0, 0, 0, 0}, {0, 1, 0, 1}},
                                 {"prn", 13, 0, PRN, {0, 0, 0, 0}, {1, 1, 0, 1}},
                                 {"rts", 14, 0, RTS, {0, 0, 0, 0}, {0, 0, 0, 0}},
                                 {"stop", 15, 0, STP, {0, 0, 0, 0}, {0, 0, 0, 0}}};

char *registries[9] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", NULL};
char *guidanceCodes[6] = {NULL, ".data", ".string", ".extern", ".entry", NULL};

/* *********** line checking and parsing *********** */

/* checks if a given line consists of only whitespace characters, returns 1 if empty, 0 otherwise */
int check_empty_line(char *line)
{
    int i = 0;
    while (line[i] != '\0')
    {
        if (!isspace(line[i]))
            return 0;
        i++;
    }
    return 1;
}

/* checks for the legality of comma placement and amount, returns 1 if legal, 0 otherwise */
int check_legal_commas(char *line, int line_num, boolean *error_found)
{
    boolean commaExpected = FALSE;
    boolean labelDec = (strchr(line, ':')) ? TRUE : FALSE;
    char *firstQ, *secondQ;
    int wordCount = 0;
    int i = 0, j = 0;

    /* check for illegal commas between label (if present) and directive/instruction */
    for (; i < strlen(line) && wordCount < ((labelDec) ? 2 : 1); i++)
    {
        if (isspace(line[i]))
            continue;
        else if (line[i] != ',' && wordCount < ((labelDec) ? 2 : 1))
        {
            /* in word */
            wordCount++;
            for (j = i; j < strlen(line) && !isspace(line[j]) && line[j] != ','; j++)
                ;
            i = j;
            if (line[i] == ',')
            {
                fprintf(stderr, "\n|Error| line:[%d] illegal comma", line_num);
                *error_found = TRUE;
                return 0;
            }
        }
        else if (line[i] == ',')
        {
            fprintf(stderr, "\n|Error| line:[%d] illegal comma", line_num);
            *error_found = TRUE;
            return 0;
        }
    }

    if (line[i] == ',')
    {
        fprintf(stderr, "\n|Error| line:[%d] illegal comma before operand", line_num);
        *error_found = TRUE;
        return 0;
    }

    firstQ = strchr(line, '"');
    secondQ = strrchr(line, '"');
    if (firstQ != NULL && secondQ != NULL && firstQ != secondQ)
        return 1;

    /* testing for legality of commas between operands */
    for (; i < strlen(line); i++)
    {
        if (!commaExpected && line[i] == ',')
        {
            fprintf(stderr, "\n|Error| line:[%d] multiple consecutive commas", line_num);
            *error_found = TRUE;
            return 0;
        }
        if (commaExpected && line[i] != ',' && !isspace(line[i]))
        {
            fprintf(stderr, "\n|Error| line:[%d] missing comma", line_num);
            *error_found = TRUE;
            return 0;
        }

        if (line[i] != ',' && !isspace(line[i])) 
        {
            for (j = i; j < strlen(line) && !isspace(line[j]) && line[j] != ','; j++)
                ;
            wordCount++;
            commaExpected = TRUE;
            i = j - 1;
            continue;
        }
        if (commaExpected && line[i] != ',' && !isspace(line[i]) && (i + 1) != strlen(line))
        {
            fprintf(stderr, "\n|Error| line:[%d] missing comma", line_num);
            *error_found = TRUE;
            return 0;
        }
        else if (commaExpected && line[i] != ',' && (i + 1) < strlen(line)) commaExpected = TRUE;
        else if ((commaExpected && line[i] == ',') || line[i] == ',') commaExpected = FALSE;
            
    }
    if (line[strlen(line) - 2] == ',')
    {
        fprintf(stderr, "\n|Error| line:[%d] illegal comma at end of line", line_num);
        *error_found = TRUE;
        return 0;
    }
    return 1;
}

/* *********** label checking and parsing *********** */

/* compares a string to known register names, returns 1 if a match is found, 0 otherwise */
int check_legal_register(char *token)
{
    int i;
    for (i = 0; registries[i]; i++)
    {
        if (strcmp(registries[i], token))
        {
            return 1;
        }
    }
    return 0;
}

/* checks for ':' in the token provided, returns the string size of label before ':' else returns 0*/
int check_label_declaration(char *token, int line_num, boolean *error_found)
{
    int i, size = 0, foundDec = FALSE;
    for (i = 0; (i <= MAX_LABEL + 1) && i < strlen(token); i++)
    {
        if (token[i] == ':')
        {
            foundDec = TRUE;
            break; /* return size; */
        }
        size++;
    }
    if (!foundDec)
        return 0;
    if (size > MAX_LABEL || size == strlen(token))
    {
        fprintf(stderr, "\n|Error| line:[%d] label size exceeds limits", line_num);
        *error_found = TRUE;
        return -1;
    }

    if (!check_legal_label(token, size, line_num, error_found))
    {
        *error_found = TRUE;
        return -1;
    }
    return size;
}

/* checks if a given label is of correct length and not overrides any guarded words, returns 1 if legal, 0 otherwise */
int check_legal_label(char *token, int labelLen, int line_num, boolean *error_found)
{
    int i = 0;

    /* iterating over the label name to check if first char is a alphabetical and the rest is alphanumerical */
    for (; i < labelLen; i++)
    {
        if ((i == 0 && !isalpha(token[i])) || !isalnum(token[i]))
        {
            fprintf(stderr, "\n|Error| line:[%d] illegal label name declaration!", line_num);
            *error_found = TRUE;
            return 0;
        }
    }
    /* checking label against guarded opCode names */
    for (i = 0; i < OP_CODES_MAX && labelLen < 5; i++)
    {
        if (!strcmp(token, opCodes[i].opName))
        {
            fprintf(stderr, "\n|Error| line:[%d] label declared as guarded word: %s", line_num, opCodes[i].opName);
            *error_found = TRUE;
            return 0;
        }
    }

    /* if label begins with an r, check if label represents registry */
    for (i = 0; *token == 'r' && registries[i] != NULL; i++)
    {
        if (!strcmp(token, registries[i]))
        {
            fprintf(stderr, "\n|Error| line:[%d] label declared as guarded word: %s", line_num, registries[i]);
            *error_found = TRUE;
            return 0;
        }
    }
    return 1;
}

/* *********** guidance checking and parsing *********** */

/* checks if a given string represents a legal guidance, returns the location of the guidance from guidanceCodes table, 0 otherwise */
int check_guidance_def(char *token, int line_num, boolean *error_found)
{
    int i = 1;
    if (strlen(token) < 5)
        return 0;

    /* comparing the given guidance with known guidances in guidanceCodes */
    while (guidanceCodes[i] != NULL && strncmp(guidanceCodes[i], token, strlen(guidanceCodes[i])))
    {
        i++;
    }

    if(i==5) return 0;

    if (strlen(token) > strlen(guidanceCodes[i]))
    {
        fprintf(stderr, "\n|Error| line:[%d] expected space after guidance", line_num);
        *error_found = TRUE;
        return 0;
    }

    return i;
}

/* collects data definition numbers into the data table, returns 1 if all data was obtained correctly, 0 otherwise */
int collect_data_arguments(char *token, dptr *dataPtr, int *DC, int line_num, boolean *error_found)
{
    int numbers;
    char *buf;
    int argCount = 0;
    while (token)
    {
        if (!isdigit(*token) && *token != '+' && *token != '-')
        {
            fprintf(stderr, "\n|Error| line:[%d] expected number guidance, data:[%s]", line_num, token);
            *error_found = TRUE;
            return 0;
        }
        numbers= (int)strtod(token, &buf);
        if(strlen(buf) != 0)
        {
            fprintf(stderr, "\n|Error| line:[%d] found trash after number:[%s]", line_num, buf);
            *error_found = TRUE;
            return 0;
        }
        add_data(dataPtr, *DC, numbers);
                
                *DC += 1;
        argCount++;
        token = strtok(NULL, " ,\t\n");
    }
    
    return 1;
}

/* collects a string into the data table, returns 1 if the string was obtained, 0 otherwise*/
int collect_string_arguments(char *token, dptr *dataPtr, int *DC, int line_num, boolean *error_found)
{
    int ch;
    int i = 1, j = 0;
    int argCount = 0;
    if (*token != '\"')
    {
        fprintf(stderr, "\n|Error| line:[%d] expected \" before string", line_num);
        *error_found = TRUE;
        return 0;
    }
    if (token[strlen(token) - 1] != '\"')
    {
        fprintf(stderr, "\n|Error| line:[%d] expected \" at the end of string token:[%s]", line_num, token);
        *error_found = TRUE;
        return 0;
    }

    while (token[i] != '\"')
    {
        ch = (int)token[i];
        add_data(dataPtr, *DC, ch);
        i++;
        j++;
        *DC += 1;
        argCount++;
    }
    ch = (int)'\0';
    add_data(dataPtr, *DC, ch);
    *DC += 1;
    argCount++;
    return 1;
}

/* *********** directive checking and parsing *********** */

/* checks if the given string represents a directive, returns a pointer to the directive from opTable, NULL otherwise */
opTable *check_directive_def(char *token, int line_num, boolean *error_found)
{
    int i = 0;
    int foundOpType = FALSE;
    for (; i < OP_CODES_MAX; i++)
    {
        if (!strcmp(token, opCodes[i].opName))
        {
            foundOpType = TRUE;
            break;
        }
    }

    if (!foundOpType)
    {
        fprintf(stderr, "\n|Error| line:[%d] Unknown directive declared for directive:[%s]", line_num, token);
        *error_found = TRUE;
        return NULL;
    }
    return &opCodes[i];
}

/* compares an instructions addressing types against valid addresing types defined in opTable, returns 1 if legal, 0 otherwise */
int check_legal_addresingTypes(opTable *currDirective, addressingType srcAdrType, addressingType destAdrType, int line_num, boolean *error_found)
{
    if (!currDirective->srcAddrTypes[srcAdrType] && srcAdrType != NONE)
    {
        fprintf(stderr, "\n|Error| line:[%d] Illegal addresing type for source operand", line_num);
        *error_found = TRUE;
        return 0;
    }
    if (!currDirective->destAddrTypes[destAdrType] && destAdrType != NONE)
    {
        fprintf(stderr, "\n|Error| line:[%d] Illegal addresing type for destination operand", line_num);
        *error_found = TRUE;
        return 0;
    }
    return 1;
}

/* collects directive arguments, returns 1 upon success, 0 otherwise */
int collect_directive_arguments(char *token, char *srcArg, addressingType *srcAdrType, char *destArg, addressingType *destAdrType, int args, int line_num, boolean *error_found)
{
    boolean collectedAllArgs = FALSE;

    if (args == 2)
    {
        token = strtok(NULL, " ,\t\n");
    }
    else
    {
        token = strtok(NULL, " \t\n");
    }
    if (!token)
    {
        fprintf(stderr, "\n|Error| line:[%d] instruction missing arguments!", line_num);
        *error_found = TRUE;
        return 0;
    }

    /* testing for registries */
    while (!collectedAllArgs)
    {
        if (*token == 'r' && strlen(token) == 2 && check_legal_register(token))
        {
            if (args == 2)
            {
                strcpy(srcArg, token);
                *srcAdrType = REG;
                args--;
                if ((token = strtok(NULL, " ,\t\n")) == NULL)
                {
                    fprintf(stderr, "\n|Error| line:[%d] missing argument", line_num);
                    *error_found = TRUE;
                    return 0;
                }
                continue;
            }
            else
            {
                strcpy(destArg, token);
                *destAdrType = REG;
                collectedAllArgs = TRUE;
            }
        }
        else if (*token == '#')
        {
            if (args == 1)
            {
                strcpy(destArg, (token) + 1);

                *destAdrType = IMM;
                collectedAllArgs = TRUE;
            }
            else
            {
                strcpy(srcArg, (token) + 1);
                *srcAdrType = IMM;
                args--;
                if ((token = strtok(NULL, " ,\t\n")) == NULL)
                {
                    fprintf(stderr, "\n|Error| line:[%d] missing argument", line_num);
                    *error_found = TRUE;
                    return 0;
                }
                continue;
            }
        }
        else if (*token == '&')
        {
            if (args == 1)
            {
                strcpy(destArg, (token) + 1);
                *srcAdrType = NONE;
                *destAdrType = RELATIVE;
                collectedAllArgs = TRUE;
            }
            else
            {
                fprintf(stderr, "\n|Error| line:[%d] too many arguments", line_num);
                *error_found = TRUE;
                return 0;
            }
        }
        else
        {
            if(!check_legal_label(token, strlen(token), line_num, error_found))
            {
                return 0;
            }
            if (args == 1)
            {
                strcpy(destArg, token);
                *destAdrType = DIRECT;
                collectedAllArgs = TRUE;
            }
            else
            {
                strcpy(srcArg, token);
                *srcAdrType = DIRECT;
                args--;
                if ((token = strtok(NULL, " ,\t\n")) == NULL)
                {
                    fprintf(stderr, "\n|Error| line:[%d] missing argument", line_num);
                    *error_found = TRUE;
                    return 0;
                }
                continue;
            }
        }
    }
    return 1;
}

/* *********** data storage and conversion functions *********** */

/* inserts an instructions into the instruction table and additional words followed by instructions, returns the amount of additional words defined:[L] */
int insert_instructions(opTable *currDirective, addressingType srcAdrType, char *srcArg, addressingType destAdrType, char *destArg, iptr *instPtr, int *IC, int line_num)
{
    int L = 0;
    int srcReg = 0, destReg = 0;
    int value = 0; /* storing value of immediate, direct or relative for additional words */

    /* checking if for additional words for source operand */
    switch (srcAdrType)
    {
    case IMM:
    case DIRECT:
        L++;
        break;
    default:
        break;
    }
    /* checking if for additional words for destination operand */
    switch (destAdrType)
    {
    case IMM:
    case DIRECT:
    case RELATIVE:
        L++;
        break;
    default:
        break;
    }

    if (srcAdrType == REG)
        srcReg = (int)strtod((srcArg) + 1, NULL);
    if (destAdrType == REG)
        destReg = (int)strtod((destArg) + 1, NULL);

    add_instruction(instPtr, *IC, currDirective->opCode, srcAdrType, srcReg, destAdrType, destReg, currDirective->functType, TRUE, FALSE, FALSE, FALSE, value, L, -1);
    if (L == 2)
    {
        if (srcAdrType == IMM)
        {
            value = (int)strtod(srcArg, NULL);
            add_instruction(instPtr, *IC + L - 1, 0, 0, 0, 0, 0, 0, TRUE, FALSE, FALSE, TRUE, value, 0, -1);
        }
        else if (srcAdrType == DIRECT)
        {
            value = 0;
            add_instruction(instPtr, *IC + L - 1, 0, 0, 0, 0, 0, 0, TRUE, TRUE, TRUE, TRUE, value, 0, line_num);
        }
        if (destAdrType == IMM)
        {
            value = (int)strtod(destArg, NULL);
            add_instruction(instPtr, *IC + L, 0, 0, 0, 0, 0, 0, TRUE, FALSE, FALSE, TRUE, value, 0, -1);
        }
        else if (destAdrType == DIRECT || destAdrType == RELATIVE)
        {
            value = line_num;
            add_instruction(instPtr, *IC + L, 0, 0, 0, 0, 0, 0, TRUE, TRUE, TRUE, TRUE, value, 0, line_num);
        }
    }
    else if (L)
    {
        if (destAdrType == IMM)
        {
            value = (int)strtod(destArg, NULL);
            add_instruction(instPtr, *IC + L, 0, 0, 0, 0, 0, 0, TRUE, FALSE, FALSE, TRUE, value, 0, -1);
        }
        else if (destAdrType == DIRECT || destAdrType == RELATIVE)
        {
            value = 0;
            add_instruction(instPtr, *IC + L, 0, 0, 0, 0, 0, 0, TRUE, TRUE, TRUE, TRUE, value, 0, line_num);
        }
        if (srcAdrType == DIRECT)
        {
            value = 0;
            add_instruction(instPtr, *IC + L, 0, 0, 0, 0, 0, 0, TRUE, TRUE, TRUE, TRUE, value, 0, line_num);
        }
        else if (srcAdrType == IMM)
        {
            value = (int)strtod(srcArg, NULL);
            add_instruction(instPtr, *IC + L, 0, 0, 0, 0, 0, 0, TRUE, FALSE, FALSE, TRUE, value, 0, -1);
        }
    }

    return L;
}

/* searches for a specific label matching:[labelName] and returns its address, otherwise returns -1 */
int find_label_address(lptr *labelPtr, char *labelName, boolean *isExtern, int line_num, boolean *error_found)
{
    int addressL = -1;
    lptr lbl = check_existing_label(labelPtr, labelName);
    if(!lbl)
    {
        fprintf(stderr, "\n|Error| line:[%d] label:[%s] for directive not declared!", line_num, labelName);
        *error_found = TRUE;
        return -1;
    }
    addressL = lbl->address;
    *isExtern = (lbl->labelType == EXTERNAL)? TRUE : FALSE;
    return addressL;
}

/* locates instructions that need to be updated with proper encoding during the second pass and updates them accordingly */
void update_instruction(iptr *instPtr, int line_num, int address, int isExtern, char *labelName)
{
    iptr p;
    if(!instPtr) 
    {
        exit(1);
    }

    p = *instPtr;
    while(!p->isTail)
    {
        if(p->A == TRUE && p->R == TRUE && p->E == TRUE && p->isData == TRUE && p->completeOnLine == line_num)
        {
            insertBinWord(p->value, address, INSTRUCTION_DATA_SIZE);
            if(isExtern == TRUE)
            {
                p->A = FALSE;
                p->R = FALSE;
                strcpy(p->labelName, labelName);
            }
            else if(isExtern == FALSE)
            {
                p->A = FALSE;
                p->E = FALSE;
                *p->labelName = '\0';
            }
            else if(isExtern == NONE)
            {
                p->R = FALSE;
                p->E = FALSE;
                *p->labelName = '\0';
            }
            p->completeOnLine = -1;
            break;
        }
        p = p->next;
        
    }
}

/* calculates the relative distance between instructions and label declarations, returns distance, 0 otherwise */
int find_relative_distance(iptr *instPtr, int labelAddress, int line_num)
{
    int distance = 0;
    int instAddress = 0;
    boolean found = FALSE;
    
    iptr inst;

    if(!instPtr)
    {
        return 0;
    }

    inst = *instPtr;
    while(!inst->isTail)
    {
        if(inst->completeOnLine == line_num && inst->A == TRUE && inst->R == TRUE && inst->E == TRUE)
        {
            instAddress = inst->address;
            found = TRUE;
            break;
        }
        inst = inst->next;
    }
    if(!found) /* checks tail */
    {
        if(inst->completeOnLine == line_num && inst->A == TRUE && inst->R == TRUE && inst->E == TRUE)
        {
            instAddress = inst->address;
            found = TRUE;
        }
    }
    distance = (labelAddress > instAddress)? labelAddress - instAddress + 1 : (instAddress - labelAddress - 1)*-1;

    return distance;
}

/* converts an instruction to a decimal for file printing, returns decimal representation of instruction */
int instruction_to_decimal(iptr *instPtr)
{
    iptr instruction;
    int i = 0;
    int j = SIZE_OF_WORD - 1;
    int data = 0;

    instruction = *instPtr;
    if (instruction->isData)
    {
        for (i = 0; i < INSTRUCTION_DATA_SIZE; i++, j--)
        {
            data += (instruction->value[i]) ? pow(2, j) : 0;
        }
    }
    else
    {
        for (i = 0; i < OPCODE_SIZE; i++, j--)
        {
            data += (instruction->opCode[i]) ? pow(2, j) : 0;
        }
        for (i = 0; i < ADDRESING_TYPE_SIZE; i++, j--)
        {
            data += (instruction->srcAdrTyp[i]) ? pow(2, j) : 0;
        }
        for (i = 0; i < REG_SIZE; i++, j--)
        {
            data += (instruction->srcReg[i]) ? pow(2, j) : 0;
        }
        for (i = 0; i < ADDRESING_TYPE_SIZE; i++, j--)
        {
            data += (instruction->dstAdrTyp[i]) ? pow(2, j) : 0;
        }
        for (i = 0; i < REG_SIZE; i++, j--)
        {
            data += (instruction->dstReg[i]) ? pow(2, j) : 0;
        }
        for (i = 0; i < FUNCT_SIZE; i++, j--)
        {
            data += (instruction->funct[i]) ? pow(2, j) : 0;
        }
    }
    data += (instruction->A) ? pow(2, 2) : 0;
    data += (instruction->R) ? pow(2, 1) : 0;
    data += (instruction->E) ? pow(2, 0) : 0;

    return data;
}

/* converts a data-node to a decimal for file printing, returns decimal representation of data-node */
int data_to_decimal(dptr *dataPtr)
{
    dptr dataNode;
    int i = 0, j = SIZE_OF_WORD - 1, data = 0;
    dataNode = *dataPtr;
    
    for (; i < SIZE_OF_WORD; i++, j--)
    {
        data += (dataNode->value[i]) ? pow(2, j) : 0;
    }
    return data;
}

/* encodes into:[word] the binary representation of:[value] in size of:[bits] */
void insertBinWord(int *word, int value, int bits)
{
    unsigned int i1;
    int i = bits + 1, word_i = 0, bits_t = bits;
    for(; i; i--,i1 = (int)pow(2,bits)){
        (i!=bits_t + 1)?word[word_i++] = (value & i1) != 0 :0;
        bits--;
    }
}