#include "fileHandler.h"




int fileHandler__read_in_file(char* filePath, char*** linesOfTextFromFile, uint32_t* numberOfLines) {

    //Open file
    if(FILEHANDLER_DEBUG)printf("\nReading in file '%s'",filePath);
    FILE *fp = fopen(filePath, "r");
    if(fp == NULL) {
        printf("\n[Filehandler]: [Error]: input file not found!");
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


    if(FILEHANDLER_DEBUG)printf("\nRead in %d lines!\n",numLines);
    fclose(fp);
    return 0;
}


uint8_t fileHandler__does_file_exist(char* filePath) {
    return access( filePath, F_OK ) == 0;
}


char* fileHandler__sterilize_text(char* text) {
    if(strlen(text) == 0)
        return NULL;
    
    uint16_t i = 0; //will step through input text
    uint16_t j = 0; //will step through toReturn, and help us to shrink it to only the needed size.
    char* toReturn = (char*) calloc(strlen(text)+1, sizeof(char));
                    
    uint8_t usedSpace = 1;//prevents copying space multiple times
    while(i < strlen(text)) {
        if(text[i] == '#' || text[i] == '\r' || text[i] == '\n')
            break;

        if(usedSpace == 0 && text[i] == ' ') {
            toReturn[j++] = ' ';
            usedSpace = 1;
        }

        if(text[i] != ' ') {
            usedSpace = 0;
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
        
        toReturn = (char*) realloc((void*) toReturn, sizeof(char) * (j+1));
        toReturn[j] = 0;
    }

    return toReturn;
}


uint8_t fileHandler__output_hex_data(struct program_token* head, char* highByteFileName, char* lowByteFileName) {
    struct program_token* token = head;
    
    //1) open files
    FILE *lowfp, *highfp;
    lowfp  = fopen(lowByteFileName, "w");
    highfp = fopen(highByteFileName, "w");
    if(lowfp == NULL || highfp == NULL) {
        printf("\n[FileHandler]: [Error]: Unable to open output files for writing! Are file permissions setup correctly?");
        return 1;
    }
    char buf[13];//Just big enough.

    //2) transfer bytes
    while(token != NULL) {
        if(token->tokenType == PROGTOK__INSTRUCTION) {
            sprintf(buf,":01%04X00%02X\n",token->romAddress,token->romData >> 8);//High
            fputs(buf,highfp);
            sprintf(buf,":01%04X00%02X\n",token->romAddress,token->romData & 0x00FF);//Low
            fputs(buf,lowfp);
        }
        token = token->nextToken;
    }

    //End of file indicators
    fputs(":00000001FF",highfp);
    fputs(":00000001FF",lowfp);
    fclose(highfp);
    fclose(lowfp);

    return 0;
}



