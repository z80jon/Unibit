#include "fileIO.h"
#include "datastructures.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>


bool bIsStartOfComment(char* text, int index);

int fileIO_readInFile(char* filePath, char*** linesOfTextFromFile, uint32_t* numberOfLines) {

    //Open file
    if(fileIO_DEBUG)printf("\nReading in file '%s'",filePath);
    FILE *fp = fopen(filePath, "r");
    if(fp == NULL) {
        printf("\n[fileIO]: [Error]: input file not found!");
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


    if(fileIO_DEBUG)printf("\nRead in %d lines!\n",numLines);
    fclose(fp);
    return 0;
}


uint8_t fileIO_doesFileExist(char* filePath) {
    return access( filePath, F_OK ) == 0;
}


//TODO add support for multiple lines and preprocessor statements using \ (but not with space after)
char* fileIO_sterilizeText(char* text) {
    if(strlen(text) == 0)
        return NULL;
    
    uint16_t i = 0; //will step through input text
    uint16_t j = 0; //will step through toReturn, and help us to shrink it to only the needed size.
    char* toReturn = (char*) calloc(strlen(text)+1, sizeof(char));
    
    bool bUsedSpace = true;//prevents copying space multiple times. Set to true so we don't copy
                           //any initial spaces.
    while(i < strlen(text) && (!bIsStartOfComment(text,i) && text[i] != '\r' && text[i] != '\n')) {

        //Space handling
        if(!bUsedSpace && (text[i] == ' ' || text[i] == '\t')) {
            toReturn[j++] = ' ';
            bUsedSpace = true;

        }
        
        if(text[i] != ' ' && text[i] != '\t') {
            bUsedSpace = false;
            toReturn[j++] = text[i];
        }

        i++;
    }

    if(j == 0) {
        //Edge case: nothing to copy (comment, blank line, etc)
        free(toReturn);
        return NULL;

    } else {
        //Edge case: whitespace before a comment or newline
        if(toReturn[j-1] == ' ')
            j--;
        
        //Shrink the buffer to only the necessary size
        toReturn = (char*) realloc((void*) toReturn, sizeof(char) * (j+1));
        toReturn[j] = 0;
    }

    return toReturn;
}


char* fileIO_extractAndCleanOperand(char* text) {

    if(strlen(text) == 0)
        return NULL;
    
    //Figure out how long the operand length is
    int operandLength = 0;
    for(int i = 0; i < 3; i++) {
        if(fileIO_containsIgnoreCase(text,OPCODE_LIST_LOAD[i])) {
            operandLength = strlen(OPCODE_LIST_LOAD[i]);
        } else if(fileIO_containsIgnoreCase(text,OPCODE_LIST_STORE[i])) {
            operandLength = strlen(OPCODE_LIST_STORE[i]);
        } else if(fileIO_containsIgnoreCase(text,OPCODE_LIST_NEGATIVE[i])) {
            operandLength = strlen(OPCODE_LIST_NEGATIVE[i]);
        } else if(fileIO_containsIgnoreCase(text,OPCODE_LIST_JUMPIFZERO[i])) {
            operandLength = strlen(OPCODE_LIST_JUMPIFZERO[i]);
        }
    }

    //Make a string small enough to not have the opcode and the space following it
    char* toReturn = (char*) calloc(strlen(text)+1-operandLength, sizeof(char));
    strcpy(toReturn,&text[operandLength+1]);

    return toReturn;
}


uint8_t fileIO_generateIntelHexFile(struct programToken_t* head, char* highByteFileName, char* lowByteFileName) {
    // struct programToken_t* token = head;
    
    // //1) open files
    FILE *lowfp, *highfp;
    lowfp  = fopen(lowByteFileName, "w");
    highfp = fopen(highByteFileName, "w");
    if(lowfp == NULL || highfp == NULL) {
        printf("\n[fileIO]: [Error]: Unable to open output files for writing! Are file permissions setup correctly?");
        return 1;
    }
    // //char buf[13];//Just big enough.

    // // //2) transfer bytes
    // // while(token != NULL) {
    // //     if(token->type == eTOKEN_INSTRUCTION) {
    // //         sprintf(buf,":01%04X00%02X\n",token->romAddress,token->romData >> 8);//High
    // //         fputs(buf,highfp);
    // //         sprintf(buf,":01%04X00%02X\n",token->romAddress,token->romData & 0x00FF);//Low
    // //         fputs(buf,lowfp);
    // //     }
    // //     token = token->nextToken;
    // // }

    // //End of file indicators
    // fputs(":00000001FF",highfp);
    // fputs(":00000001FF",lowfp);
    // fclose(highfp);
    // fclose(lowfp);

    return 0;
}


bool fileIO_hasFileBeenImportedAlready(char* filePath) {
    //NYI
    //TODO implement
    return false;
}


bool fileIO_equalsIgnoreCase(char const *str1, char const *str2)
{
    if(strlen(str1) != strlen(str2))
        return false;


    for (int index = 0; index < strlen(str2); index++) {
        if((tolower((unsigned char)str1[index]) - tolower((unsigned char)str2[index])) != 0)
            return false;
    }

    return true;
}

bool fileIO_containsIgnoreCase(char const *str1, char const *str2)
{       
    //Edge case: str1 is shorter than str2
    if(strlen(str1) < strlen(str2))
        return false;
    
    for (int index = 0; index < strlen(str2); index++) {
        if((tolower((unsigned char)str1[index]) - tolower((unsigned char)str2[index])) != 0)
            return false;
    }

    //If string 1 is longer than string 2, make sure that this isn't just a sub-word for which
    //string 1 contains it (but the text in 1 goes on further), eg str1 = "apple", str2 = "a", we
    //want to return false--so we must test str1[1].
    if(strlen(str1) > strlen(str2)) {
        if(str1[strlen(str2)] != ' ')
            return false;
    }

    return true;
}


/**
 * \brief Returns true if the char at text[index] is the first '/' in a '//' comment start
 * 
 * \param text the string to check
 * \param index the index within the string 'text' to check
 * \return true if it is the beginning of a comment
 * \return false if it is not the beginning of a comment
 */
bool bIsStartOfComment(char* text, int index) {
    return (strlen(text) > (index + 1)) && text[index] == '/' && text[index+1] == '/';
}