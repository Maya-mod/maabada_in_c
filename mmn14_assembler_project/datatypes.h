#ifndef DATATYPES_H_
#define DATATYPES_H_
#include <stdlib.h>
#include "definitions.h"
typedef enum boolean {FALSE,TRUE} boolean;
typedef enum addressingType {IMM,DIRECT,RELATIVE,REG,NONE} addressingType;
typedef enum labelType {DATA=1,CODE,EXTERNAL} labelType;
typedef enum opName{MOV = 1, CMP, ADD, SUB, LEA, CLR, NOT, INC, DEC, JMP, BNE, JSR, RED, PRN, RTS, STP} opName;

/* defining a struct that holds label information*/
typedef struct labelNode * lptr;
typedef struct labelNode{
    char name[MAX_LABEL + 1];
    labelType labelType;
    int isEntry;
    int address;
    int isUsed;
    boolean isTail;
    lptr next;
} labelNode;

/* defining a strcut that holds data definitions */
typedef struct dataTable *dptr;
typedef struct dataTable{
    int address; /* DC Counter */
    int value[SIZE_OF_WORD]; /* Binary code */
    boolean isTail;
    dptr next;
} dataTable;

/* defining a struct that holds instructions */
typedef struct instructionTable *iptr;
typedef struct instructionTable{
    int address;
    int opCode[OPCODE_SIZE];
    int srcAdrTyp[ADDRESING_TYPE_SIZE];
    int srcReg[REG_SIZE];
    int dstAdrTyp[ADDRESING_TYPE_SIZE];
    int dstReg[REG_SIZE];
    int funct[FUNCT_SIZE];
    int A;
    int R;
    int E;
    boolean isData;
    int value[INSTRUCTION_DATA_SIZE];
    char labelName[MAX_LABEL];
    int completeOnLine;
    int L;
    boolean isTail;
    iptr next;
} instructionTable;

/* defining a struct that holds opcode information */
typedef struct opTable
{
    char opName[OP_CODE_MAX_SIZE];
    int opCode;
    int functType;
    opName opNum;
    addressingType srcAddrTypes[4];
    addressingType destAddrTypes[4];
} opTable;

#endif