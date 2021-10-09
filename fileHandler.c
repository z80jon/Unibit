#include "fileHandler.h"

int fileHandler_readInFile(char* fileName, char*** linesOfTextFromFile, uint32_t* numberOfLines) {

    //Open file
    printf("\nReading in file '%s'",fileName);
    FILE *fp = fopen(fileName, "r");
    char buff[255];
    if(fp == NULL) {
        printf("\nERROR: input file not found!");
        return 1;
    }

    //Read in # lines
    uint32_t numLines = 1;
    int c = getc(fp);
    //Empty file edge case
    if(c == EOF)
        numLines = 0;
    while(c != EOF) {
        if(c == '\n')
            numLines++;
        c = getc(fp);
    }
    
    //malloc pointers to lines
    *linesOfTextFromFile = (char**) malloc(sizeof(char**) * numLines);

    //Determine number of characters in each line for malloc purposes, and malloc.
    fseek(fp, 0, SEEK_SET);
    for(uint32_t i = 0; i < numLines; i++) {

        uint32_t numChars = 1;
        c = getc(fp);
        if(c == '\n' || c == EOF)
            numChars = 0;
        while(c != EOF && c != '\n') {
            c = getc(fp);
            numChars++;
        }

        //printf("\nNum chars in line %d: %d",i,numChars);

        linesOfTextFromFile[0][i] = (char*) calloc(numChars+1, sizeof(char));//room for null term
    }

    //Finally, copy over each and every line.
    fseek(fp, 0, SEEK_SET);
    for(uint32_t i = 0; i < numLines; i++) {
        
        //Only copy if not /n
        uint32_t j = 0;
        c = getc(fp);
        if(!(c == '\n' || c == EOF)) {
            while(c != EOF && c != '\n') {
                linesOfTextFromFile[0][i][j] = c;
                c = getc(fp);
                j++;
        }
        } 
    }

    *numberOfLines = numLines;


    printf("\nRead in %d lines!\n",numLines);
    fclose(fp);
    return 0;
}