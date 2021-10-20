#include "fileHandler.h"

int fileHandler_readInFile(char* filePath, char*** linesOfTextFromFile, uint32_t* numberOfLines) {

    //Open file and copy its contents to unsterilizedFileText
    printf("\nReading in file '%s'",filePath);
    FILE *fp = fopen(filePath, "r");
    if(fp == NULL) {
        printf("\nERROR: input file not found!");
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    size_t fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* unsterilizedFileText = (char*)calloc(fileSize+1, sizeof(char));
    for(long i = 0; i < fileSize; i++) {
        unsterilizedFileText[i] = getc(fp);
    }

    //TODO change to the following:
    //1) read in entire file
    //2) in cases of a comment start, whitespace out until the next /r or /n is seen. Whitespace them out too.
    //3) all /r -> ' ', all '\n' -> ';.
    fclose(fp);
    return 0;
}

uint8_t fileHandler_doesFileExist(char* filePath) {
    return access( filePath, F_OK ) == 0;
}


char* fileHandler_sterilizeText(char* text) {
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