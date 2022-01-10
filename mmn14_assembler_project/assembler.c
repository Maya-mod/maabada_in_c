/* Program Name: assembler
** Performs assembly proccess on source files and translates the given source files into object, extern and entry files as defined in the project book
** Authors: Maya ****, Gabi ****
** Course: 20465
** Semester: B 
** Date: 23/08/2020
**/

#include "assembler.h"
#include <string.h>
#include <math.h>

/* attemps to open a file for reading mode:[mode] with the given suffix:[suffix] for filename:[name] */
FILE *fopen_f(char *name, char *suffix, char *mode)
{
    FILE *fp;
    char *fileSize = (char *)malloc(strlen(name) + strlen(suffix) + 1), *file = fileSize;
    sprintf(file, "%s%s", name, suffix);
    fp = fopen(file, mode);
    free(fileSize);
    return fp;
}

/* creates neccesary files for given assembly file */
int print_files(lptr *labelPtr, dptr *dataPtr, iptr *instPtr, char *filename, int IC, int DC)
{
    /* printing object file */
    FILE *obFp = fopen_f(filename, OBJECT_SUFFIX, "w");
    FILE *entFp, *extFp;
    int linesToPrint = IC - MEMORY_ADDRESS_OFFSET + DC;
    int i = 0, data = 0;
    boolean externFile = FALSE, entryFile = FALSE;
    lptr currLabel;
    dptr currData;
    iptr currInst;

    if (!obFp)
    {
        fprintf(stderr, "|Error| Could not create object file:[%s%s]", filename, OBJECT_SUFFIX);
        return 0;
    }

    /* checking if extern files are needed and if entry files are needed */
    currLabel = *labelPtr;
    while (currLabel)
    {
        if (currLabel->isEntry)
            entryFile = TRUE;
        if (currLabel->labelType == 3)
            externFile = TRUE;
        if (entryFile && externFile)
            break;
        currLabel = currLabel->next;
    }

    /* printing object file */
    currInst = *instPtr;
    currData = *dataPtr;
    fprintf(obFp, "%7d %d\n", IC - MEMORY_ADDRESS_OFFSET, DC);
    for (; i < IC - MEMORY_ADDRESS_OFFSET && currInst; i++)
    {
        data = instruction_to_decimal(&currInst);
        fprintf(obFp, "%07d %06x\n", currInst->address, data);
        currInst = currInst->next;
    }
    for (; i < linesToPrint && currData; i++)
    {
        data = data_to_decimal(&currData);
        fprintf(obFp, "%07d %06x\n", currData->address, data);
        currData = currData->next;
    }
    fclose(obFp);

    /* printing entries file */
    if (entryFile)
    {
        entFp = fopen_f(filename, ENTRY_SUFFIX, "w");
        if (!entFp)
        {
            fprintf(stderr, "|Error| Could not create entry file:[%s%s]", filename, ENTRY_SUFFIX);
            return 0;
        }
        currLabel = *labelPtr;
        while (currLabel)
        {
            if (currLabel->isEntry)
            {
                fprintf(entFp, "%s %07d\n", currLabel->name, currLabel->address);
            }
            currLabel = currLabel->next;
        }
        fclose(entFp);
    }

    /* printing extern file */
    if (externFile)
    {
        extFp = fopen_f(filename, EXTERN_SUFFIX, "w");
        if (!entFp)
        {
            fprintf(stderr, "|Error| Could not create extern file:[%s%s]", filename, EXTERN_SUFFIX);
            return 0;
        }
        currInst = *instPtr;
        while (currInst)
        {
            if (*currInst->labelName != '\0')
            {
                fprintf(extFp, "%s %07d\n", currInst->labelName, currInst->address);
            }
            currInst = currInst->next;
        }
        fclose(extFp);
    }
    return 1;
}

int main(int argc, char *argv[])
{
    int i;
    FILE *fp;
    int IC, DC;
    lptr labelPtr;
    dptr dataPtr;
    iptr instPtr;

    if (argc < 2)
    {
        printf("|Error| No file names were passed to assemble!!");
        return 1;
    }
    for (i = 1; i < argc; i++)
    {
        fp = fopen_f(argv[i], ASSEMBLY_SUFFIX, "r");
        if (!fp)
        {
            fprintf(stderr, "|Error| Could not open file |%s%s|", argv[i],ASSEMBLY_SUFFIX);
            return 0;
        }

        IC = 100, DC = 0;
        labelPtr = NULL;
        dataPtr = NULL;
        instPtr = NULL;
        printf("|Assembler| Starting assembly for source file %s%s", argv[i], ASSEMBLY_SUFFIX);
        printf("\n|Assembler| Starting firstPass ");
        if (firstPass(fp, &IC, &DC, &labelPtr, &dataPtr, &instPtr))
        {
            printf("\n|Assembler| ****found errors in firstpass, ABORTING!!!**** \n");
            fclose(fp);
            free_nodes(&labelPtr, &dataPtr, &instPtr);
            fflush(NULL);
            exit(1);
        }
        rewind(fp);
        printf("\n|Assembler| Starting second pass");
        if (secondPass(fp, &IC, &DC, &labelPtr, &dataPtr, &instPtr))
        {
            printf("\n|Assembler| ****found errors in secondpass, ABORTING!!!**** \n");
            fclose(fp);
            free_nodes(&labelPtr, &dataPtr, &instPtr);
            fflush(NULL);
            exit(1);
        }
        fclose(fp);
        printf("\n|Assembler| Passed passes successfully, exporting files\n");
        print_files(&labelPtr, &dataPtr, &instPtr, argv[i], IC, DC);   
        free_nodes(&labelPtr, &dataPtr, &instPtr);
        fflush(NULL);
    }

    return 0;
}