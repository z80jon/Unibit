#include "tokenizer.h"


struct program_token* tokenizer_tokenize(char* inputFile) {

    uint32_t numLines;
    char** linesOfText;
    fileHandler_readInFile(inputFile,&linesOfText, &numLines);

    struct program_token* head = (struct program_token*)malloc(sizeof(struct program_token));
    struct program_token* pointer;
    struct program_token* next;
    uint16_t currentLine = 0;
    head->prevToken = NULL;
    head->nextToken = NULL;
    head->instruction_text = NULL;
    pointer = head;

    //Step 1: create linked list of tokens of each part of the program (instructions, labels, constants, etc.)
    while(currentLine < numLines) {
        char* sterilizedLineText = fileHandler_sterilizeText(linesOfText[currentLine]);//remove any comments, extra spaces, etc.
    //TODO record line number
        if(sterilizedLineText != NULL) {

            if(tokenizer_hasLabel(sterilizedLineText)) {
                next = tokenizer_makeLabelToken(sterilizedLineText);//TODO grab the token!
                for(uint8_t i = 0; i < strlen(sterilizedLineText); i++) {
                    if(sterilizedLineText[i] == ' ') {
                        sterilizedLineText = &sterilizedLineText[i+1];
                        break;
                    }
                }
                pointer->nextToken = next;
                next->prevToken = pointer;
                next->lineNumber = currentLine;
            }
            
            if(tokenizer_hasPreprocessorDirective(sterilizedLineText)) {
                next = tokenizer_makePreprocessorToken(sterilizedLineText);//TODO grab the token!
                pointer->nextToken = next;
                next->prevToken = pointer;
                next->lineNumber = currentLine;
            } else if(tokenizer_hasOpcode(sterilizedLineText)) {
                next = tokenizer_makeOpcodeToken(sterilizedLineText);//TODO grab the token!
                pointer->nextToken = next;
                next->prevToken = pointer;
                next->lineNumber = currentLine;
            }

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


uint8_t tokenizer_hasOpcode(char* c) {
    char* buf;
    for(uint8_t i = 0; i < OPCODE_STRINGS_LENGTH; i++) {
        buf = strstr(c, OPCODE_STRINGS[i]);
        if(buf != NULL)
            return 1;
    }
    return 0;
}

struct program_token* tokenizer_makePreprocessorToken(char* string) {
    struct program_token* toReturn = (struct program_token*)calloc(1, sizeof(struct program_token));
    toReturn->prevToken = NULL;
    toReturn->nextToken = NULL;
    toReturn->address = 0;
    toReturn->instruction_text = (char*)calloc(strlen(string)+1, sizeof(char));
    toReturn->typeOfToken = PREPROCESSOR_DIRECTIVE;
    strcpy(toReturn->instruction_text, string);
    
    return toReturn;
}


struct program_token* tokenizer_makeLabelToken(char* string) {
    struct program_token* toReturn = (struct program_token*)calloc(1, sizeof(struct program_token));
    toReturn->prevToken = NULL;
    toReturn->nextToken = NULL;
    toReturn->address = 0;
    toReturn->instruction_text = (char*)calloc(strlen(string)+1, sizeof(char));
    toReturn->typeOfToken = LABEL;
    strcpy(toReturn->instruction_text, string);

    //Simple fix to ensure that if an opcode does follow the label, code referencing this will ignore it.
    for(uint8_t i = 0; i < strlen(string); i++) {
        if(toReturn->instruction_text[i] == ' ') {
            toReturn->instruction_text[i] = '\0';
            break;
        }
    }

    return toReturn;
}


struct program_token* tokenizer_makeOpcodeToken(char* string) {
    struct program_token* toReturn = (struct program_token*)calloc(1, sizeof(struct program_token));
    toReturn->prevToken = NULL;
    toReturn->nextToken = NULL;
    toReturn->address = 0;
    toReturn->instruction_text = (char*)calloc(strlen(string)+1, sizeof(char));
    toReturn->typeOfToken = INSTRUCTION;
    strcpy(toReturn->instruction_text, string);

    return toReturn;
}

