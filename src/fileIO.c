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
#include "utils.h"




int removeChar(char** text, int index);


int fileIO_readInFile(char* filePath, char** fileText) {

    //Open file
    if(fileIO_DEBUG)printf("\nReading in file '%s'",filePath);
    FILE *fp = fopen(filePath, "r");
    if(fp == NULL) {
        printf("\n[fileIO]: [Error]: input file not found!");
        return 1;
    }

    //Read in the number of characters
    fseek(fp, 0L, SEEK_END);
    uint32_t numChars = ftell(fp);
    
    //malloc string for file text
    *fileText = (char*) malloc(sizeof(char) * numChars);

    //Finally, copy over the text
    fseek(fp, 0, SEEK_SET);
    fread(*fileText, 1, numChars, fp);

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

    for(int i = 0; i < strlen(text); i++) {
        
        //Remove redundant whitespaces or initial whitespaces
        //if( || (i == 0 && bIsWhitespaceDelimiter(text[i])))
    }
    
    uint16_t i = 0; //will step through input text
    uint16_t j = 0; //will step through toReturn, and help us to shrink it to only the needed size.
    char* toReturn = (char*) calloc(strlen(text)+1, sizeof(char));
    
    bool bUsedSpace = true;//prevents copying space multiple times. Set to true so we don't3 copy
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
 * \brief Given a pointer to an array, remove the character at index "index" and shift all following characters down.
 *        Then, reallocate the array accordingly.
 * 
 * \param text 
 * \param index 
 * \return int 
 */
int removeChar(char** text, int index) {
    if(index > strlen(*text) || index < 0)
        return 1;

    for(int i = index; i < strlen(*text) - 1; i++) {
        *text[i] = *text[i+1];
    }

    //Shrink the allocated portion of memory by 1
    *text = realloc(*text, strlen(*text) * sizeof(char));
}


