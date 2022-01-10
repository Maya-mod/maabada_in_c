#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_
#include "datatypes.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

opTable opCodes[OP_CODES_MAX];
char *registries[9];
char *guidanceCodes[6];

/* *********** line checking and parsing *********** */

int check_empty_line(char *);
int check_legal_commas(char *, int, boolean *);

/* *********** label checking and parsing *********** */

int check_label_declaration(char *, int, boolean *);
int check_legal_label(char *, int, int, boolean *);

/* *********** guidance checking and parsing *********** */

int check_guidance_def(char *, int, boolean *);
int collect_data_arguments(char *, dptr *, int *, int, boolean *);
int collect_string_arguments(char *, dptr *, int *, int, boolean *);

/* *********** directive checking and parsing *********** */

opTable * check_directive_def(char *, int, boolean *);
int check_legal_addresingTypes(opTable *, addressingType, addressingType, int, boolean *);
int collect_directive_arguments(char *, char *, addressingType *, char *, addressingType *, int, int, boolean *);

/* *********** data storage and conversion functions *********** */

int insert_instructions(opTable *, addressingType, char *, addressingType, char *, iptr *, int *, int);
int find_label_address(lptr *, char *, boolean *, int, boolean *);
void update_instruction(iptr *, int, int, int, char *);
int find_relative_distance(iptr *, int, int);
void insertBinWord(int *word, int value, int bits);
int instruction_to_decimal(iptr *);
int data_to_decimal(dptr *);

/* *********** linked list functions *********** */

extern int add_label(lptr *, char *, int, labelType, int, int, boolean *);
extern lptr check_existing_label(lptr *, char  *);
extern int add_data(dptr *, int, int);
extern int add_instruction(iptr *, int, int, int, int, int, int, int, int, int, int, boolean, int, int, int);
extern int mark_extern_label(lptr *, char *, int, boolean *);
extern int mark_entry_label(lptr *, char *, int, boolean *);
extern int update_dc_tables(lptr *, dptr *, int *);
extern void free_nodes(lptr *, dptr *, iptr *);

#endif