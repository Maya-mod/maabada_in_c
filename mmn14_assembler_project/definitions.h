#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_
/* Constants */
#define MAX_LINE 82                     /* max line length for 80 chars, \n and \0 */
#define MAX_LABEL 31                    /* max label name length */
#define MEMORY_ADDRESS_OFFSET 100

#define OP_CODES_MAX 16                 /* number of opcodes available for the assembler for use */
#define OP_CODE_MAX_SIZE 5              /* max string size of opcode */

/* Memory word field sizes */
#define SIZE_OF_WORD 24                 /* bit count of word: 24 bits */
#define INSTRUCTION_DATA_SIZE 21        /* bit count of instruction's additional words, 24 bits - ARE */
#define OPCODE_SIZE 6                   /* bit count of opcode */
#define ADDRESING_TYPE_SIZE 2           /* bit count of addresing type */
#define REG_SIZE 3                      /* bit count of registers */
#define FUNCT_SIZE 5                    /* bit count of funct */

/* File extensions */
#define ASSEMBLY_SUFFIX ".as"           /* input file assembly suffix */
#define OBJECT_SUFFIX ".ob"             /* output file object suffix */
#define ENTRY_SUFFIX ".ent"             /* output file entry suffix */
#define EXTERN_SUFFIX ".ext"            /* output file extern suffix */

#endif