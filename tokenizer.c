#include "tokenizer.h"


struct program_token* tokenizer_tokenize(char* inputFile) {

    uint32_t numLines;
    char** linesOfText;
    fileHandler_readInFile(inputFile,&linesOfText, &numLines);

    struct program_token* head = (struct program_token*)malloc(sizeof(struct program_token));
    struct program_token* pointer;
    uint16_t currentLine = 0;
    head->prevToken = NULL;
    head->nextToken = NULL;
    head->instruction_text = NULL;
    pointer = head;

    //Step 1: create linked list of tokens of each part of the program (instructions, labels, constants, etc.)
    while(currentLine < numLines) {
        char* sterilizedLineText = fileHandler_sterilizeText(linesOfText[currentLine]);//remove any comments, extra spaces, etc.

        if(sterilizedLineText != NULL) {

            if(tokenizer_hasLabel(sterilizedLineText)) {
                //TODO
            }
            
            if(tokenizer_hasPreprocessorDirective(sterilizedLineText)) {
                //TODO
            } else if(tokenizer_hasOpcode(sterilizedLineText)) {
                //TODO
            }

            pointer->instruction_text = sterilizedLineText;
            struct program_token* next = (struct program_token*)calloc(1,sizeof(struct program_token));      
            pointer->nextToken = next;
            next->prevToken = pointer;
            pointer = next;
            pointer->instruction_text = NULL;
            pointer->nextToken = NULL;
        }

        currentLine++;
    }

    for(uint16_t i = 0; i < numLines; i++) {

    }

    return head;
}


uint8_t tokenizer_hasLabel(char* c) {
    uint8_t index = 0;
    while(c[index] != '\0' && c[index] != ' ' && c[index] != '\r' && c[index] != '\n') {
        if(c[index] == ':')
            return 1;
    }
    return 0;
}

uint8_t tokenizer_hasPreprocessorDirective(char* c) {
    return c[0] == '.';
}


const char** OPCODE_STRINGS = {"l ", "load", "s", "store", "negate", "n", "jumpifzero", "jiz"};
#define OPCODE_STRINGS_LENGTH 8
uint8_t tokenizer_hasOpcode(char* c) {
    char* buf;
    for(uint8_t i = 0; i < OPCODE_STRINGS_LENGTH; i++) {
        buf = strstr(c, OPCODE_STRINGS[i]);
        if(buf != NULL)
            return 1;
    }
    return 0;
}