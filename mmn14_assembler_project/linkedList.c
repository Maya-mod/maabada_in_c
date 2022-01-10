#include "assembler.h"
#include <string.h>

/* add a node to the label table */
int add_label(lptr *labelNodePtr, char *labelName, int address, labelType type, int isEntry, int line_num, boolean *error_found)
{
    lptr head, t;
    t = (lptr)malloc(sizeof(labelNode));
    if (!t)
    {
        printf("|Error| Space allocation error!");
        return 1;
    }
    head = *labelNodePtr;
    strcpy(t->name, labelName);
    t->address = address;
    t->labelType = type;
    t->isEntry = isEntry;
    t->next = NULL;
    t->isTail = TRUE;

    if (!head)
    {
        *labelNodePtr = t;
    }
    else
    {
        while (!head->isTail)
        {
            if(!strcmp(t->name, head->name))
            {
                fprintf(stderr, "\n|Error| line:[%d] found matching label already present", line_num);
                *error_found = TRUE;
                free(t);
                return 1;
            }
            head = head->next;
        }
        head->next = t;
        head->isTail = FALSE;
    }
    return 0;
}

/* add data-node to data table */
int add_data(dptr *dataPtr, int address, int value)
{
    dptr head, t;
    t = (dptr)malloc(sizeof(dataTable));
    if(!t)
    {
        printf("\n|Error| Space allocation error!");
        exit(1);
    }
    head = *dataPtr;
    t->address = address;
    insertBinWord(t->value, value, SIZE_OF_WORD);
    t->isTail = TRUE;
    if(!head)
    {
        *dataPtr = t;
    }else
    {
        while(!head->isTail)
        {
            head = head->next;
        }
        head->next = t;
        head->isTail = FALSE;
    }
    return 1;
}

/* add an instruction to the instruction table */
int add_instruction(iptr *instPtr, int address, int opCode, int srcAdrTyp, int srcReg, int dstAdrTyp, int dstReg, int funct, int A, int R, int E, boolean isData, int value, int L, int completeOnLine)
{
    iptr head, t;

    t = (iptr)malloc(sizeof(instructionTable));
    if(!t)
    {
        printf("\n|Error| Space allocation error!");
        return 0;
    }

    t->address = address;
    if(isData)
    {
        insertBinWord(t->value, value, INSTRUCTION_DATA_SIZE);
    }
    else
    {
        insertBinWord(t->opCode, opCode, OPCODE_SIZE);
        insertBinWord(t->srcAdrTyp, srcAdrTyp, ADDRESING_TYPE_SIZE);
        insertBinWord(t->srcReg, srcReg, REG_SIZE);
        insertBinWord(t->dstAdrTyp, dstAdrTyp, ADDRESING_TYPE_SIZE);
        insertBinWord(t->dstReg, dstReg, REG_SIZE);
        insertBinWord(t->funct, funct, FUNCT_SIZE);
    }
    
    t->A = A;
    t->R = R;
    t->E = E;
    t->isData = isData;
    t->L = L;
    t->completeOnLine = completeOnLine;
    t->isTail = TRUE;
    head = *instPtr;
    if(!head)
    {
        *instPtr = t;
    }else
    {
        while(!head->isTail)
        {
            head = head->next;
        }
        head->next = t;
        head->isTail = FALSE;
    }
    return 1;
}

/* locates existing label and returns a pointer to that label, else returns NULL */
lptr check_existing_label(lptr *labelNodePtr, char  *labelName)
{
    lptr p;
    if(!labelNodePtr) return NULL; /* check if head is initiated */
    p = *labelNodePtr;
    while(p)
    {
        if(!strcmp(labelName, p->name)) return p;
        p = p->next;
    }
    return NULL;
}

/* locates a specific label and marks it external, if not found attempts to add label to labeltable */
int mark_extern_label(lptr *labelPtr, char *labelName, int line_num, boolean *error_found)
{
    lptr p = check_existing_label(labelPtr, labelName);
    if(!p)
    {
        add_label(labelPtr, labelName, 0, 2, 0, line_num, error_found);
    }
    else
    {
        p->labelType = 2;
    }
    return 1;
}

/* locates a label from the labeltable and marks it entry, if not found returns error */
int mark_entry_label(lptr *labelPtr, char *labelName, int line_num, boolean *error_found)
{
    lptr p = check_existing_label(labelPtr, labelName);
    if(!p)
    {
        fprintf(stderr, "\n|Error| line:[%d] label:[%s] not declared!", line_num, labelName);
        *error_found = TRUE;
        return 1;
    }
    else
    {
        p->isEntry = 1;
    }
    return 0;
}

/* updates the label table and data-node table with the final IC after the first pass */
int update_dc_tables(lptr *labelPtr, dptr *dataPtr, int *IC)
{
    lptr l1 = NULL;
    dptr d1 = NULL;
    
    if(!labelPtr || !dataPtr)
    {
        fprintf(stderr, "\n|Error| no tables present");
        return 0;
    }
    l1 = *labelPtr;
    d1 = *dataPtr;

    /* updating labels addresses marked DATA with IC */
    while(l1)
    {
        if(l1->labelType == DATA)
        {
            l1->address += *IC;
        }
        l1 = l1->next;
    }

    /* updating data nodes addresses with IC */
    while(d1)
    {
        d1->address += *IC;
        d1 = d1->next;
    }
    return 1;
}

/* free table data allocated by malloc from memory */
void free_nodes(lptr *labelPtr, dptr *dataPtr, iptr *instPtr)
{
    lptr l1, l2;
    iptr i1, i2;
    dptr d1, d2;

    l1 = *labelPtr;
    i1 = *instPtr;
    d1 = *dataPtr;

    /* Release labels from memory */
    while(!l1->isTail)
    {
        l2 = l1->next;
        free(l1);
        l1 = NULL;
        l1 = l2;
    }
    free(l1);
    l1 = NULL;

    /* Release instructions from memory */
    while(!i1->isTail)
    {
        i2 = i1->next;
        free(i1);
        i1 = NULL;
        i1 = i2;
    }
    free(i1);
    i1 = NULL;

    /* Release data declarations from memory */
    while(!d1->isTail)
    {
        d2 = d1->next;
        free(d1);
        d1= NULL;
        d1 = d2;
    }
    free(d1);
    d1 = NULL;

    *labelPtr = NULL;
    *dataPtr = NULL;
    *instPtr = NULL;
}