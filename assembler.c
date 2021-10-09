#include "assembler.h"

void assembler_assemble(char* inputFile, char* outputFile) {

    uint16_t numLines;
    char** linesOfText;
    fileHandler_readInFile(inputFile,&linesOfText, &numLines);


    struct instruction_token head = assembler_tokenizeText(linesOfText, numLines);
    // {
    // printf("\n====ASSEMBLING %d LINES====\n\n",numLines);
    //  int32_t lineLength;

    // for(int32_t i = 0; i < numLines; i++) {
    //     printf("Line %d: \"%s\"",i,lines[i]);
    //     switch(toupper(lines[i][0])) {
    //         case 'N':
    //             printf(" --> NGA");
    //             break;
            
    //         case 'L':
    //             lineLength = strlen(lines[i]);
    //             if(lineLength < 3)
    //                 printf("\n\nAssembler error: missing address to load from");
    //             printf(" --> LOAD ");
    //     }

    //     printf("\n");
    // }
}


struct instruction_token assembler_tokenizeText(char** text, uint16_t numLines) {
    struct instruction_token head;
    struct instruction_token* pointer;
    pointer = &head;
    uint16_t currentLine = 0;
    head.nextToken = NULL;
    head.instruction_text = NULL;

    while(currentLine < numLines) {

        char* temp = assembler_tokenizeLine(text[currentLine]);

        if(strlen(temp > 0)) {
            pointer->instruction_text = temp;
            struct instruction_token* next = (struct instruction_token*)malloc(sizeof(struct instruction_token));            
            pointer->nextToken = next;
            pointer = next;
            pointer->instruction_text = NULL;
            pointer->nextToken = NULL;
        }

        currentLine++;
    }

    return head;
}

char* assembler_tokenizeLine(char* text) {
    if(strlen(text) == 0)
        return NULL;
    
    uint16_t i = 0; //will step through input text
    uint16_t j = 0; //will step through toReturn, and help us to shrink it to only the needed size.
    char* toReturn = (char*) malloc(sizeof(char) * strlen(text));
                    
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
