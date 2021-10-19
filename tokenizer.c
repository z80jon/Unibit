#include "tokenizer.h"


struct program_token* tokenizer_tokenize(char* inputFile) {

    uint32_t numLines;
    char** linesOfText;
    fileHandler_readInFile(inputFile,&linesOfText, &numLines);

    struct program_token* head = (struct program_token*)calloc(1,sizeof(struct program_token));
    struct program_token* pointer;
    struct program_token* next;
    uint16_t currentLine = 0;
    uint16_t romAddress = 0;
    head->prevToken = NULL;
    head->nextToken = NULL;
    head->instruction_text = NULL;
    head->tokenType = PROGTOK__HEAD;
    pointer = head;

    //Step 1: create linked list of tokens of each part of the program (instructions, labels, constants, etc.)
    while(currentLine < numLines) {
        char* sterilizedLineText = fileHandler_sterilizeText(linesOfText[currentLine]);//remove any comments, extra spaces, etc.
    //TODO record line number
        uint8_t lineHadLabel = 0;
        if(sterilizedLineText != NULL) {


            if(tokenizer_hasLabel(sterilizedLineText)) {
                printf("\nline %d: \"%s\" --> Label",currentLine+1,sterilizedLineText);
                next = tokenizer_makeLabelToken(sterilizedLineText);
                for(uint8_t i = 0; i < strlen(sterilizedLineText); i++) {
                    if(sterilizedLineText[i] == ' ') {
                        sterilizedLineText = &sterilizedLineText[i+1];
                        break;
                    }
                }

                pointer->nextToken = next;
                next->prevToken = pointer;
                next->lineNumber = currentLine;
                pointer = next;
                lineHadLabel = 1; //Workaround so no error message prints if there's a label on a line by itself.
            }
            
            if(tokenizer_hasPreprocessorDirective(sterilizedLineText)) {
                printf("\nLine %d: \"%s\" --> Preprocessor Directive",currentLine+1,sterilizedLineText);
                next = tokenizer_makePreprocessorToken(sterilizedLineText);

            } else if(tokenizer_hasOpcode(sterilizedLineText)) {
                printf("\nLine %d: \"%s\" --> OpCode",currentLine+1,sterilizedLineText);
                next = tokenizer_makeOpcodeToken(sterilizedLineText);
                next->romAddress = romAddress++;//we only assign ROM addresses to opcodes
                
            } else if(tokenizer_hasVariable(sterilizedLineText)) {
                printf("\nLine %d: \"%s\" --> Variable",currentLine+1,sterilizedLineText);
                next = tokenizer_makeVariableDeclarationToken(sterilizedLineText);
                
            } else if(!lineHadLabel){
                printf("\n[Tokenizer]: [ERROR]: Unable to determine what type of token '%s' is!",sterilizedLineText);
                //return NULL;
            }

            pointer->nextToken = next;
            next->prevToken = pointer;
            next->lineNumber = currentLine;
            pointer = next;
        }

        currentLine++;
    }

    //deallocate memory for the file text we just finished processing
    for(uint16_t i = 0; i < numLines; i++) {
        free(linesOfText[i]);
    }
    free(linesOfText);

    return head;
}


uint8_t tokenizer_hasLabel(char* c) {
    uint8_t index = 0;
    while(c[index] != '\0' && c[index] != ' ' && c[index] != '\r' && c[index] != '\n' && index < strlen(c)) {
        if(c[index++] == ':')
            return 1;
    }
    return 0;
}


uint8_t tokenizer_hasPreprocessorDirective(char* c) {
    return c[0] == '.';
}


uint8_t tokenizer_hasOpcode(char* c) {
    char* buff = calloc(strlen(c)+1, sizeof(char));
    strcpy(buff, c);
    buff = strtok(buff, " ");//Grab first word of string
    for(uint8_t i = 0; i < OPCODE_STRINGS_LENGTH; i++) {
        if(strcmp(buff, OPCODE_STRINGS[i]) == 0) {
            free(buff);
            return 1;
        }
    }
    free(buff);
    return 0;
}


uint8_t tokenizer_hasVariable(char* c) {
    return strstr(c, "var") == c;
}


struct program_token* tokenizer_makePreprocessorToken(char* string) {
    struct program_token* toReturn = tokenizer_makeGenericToken(string, PROGTOK__PREPROC_DIR);
    
    return toReturn;
}


struct program_token* tokenizer_makeLabelToken(char* string) {
    char* buff = (char*)calloc(strlen(string)+1,sizeof(char));
    strcpy(buff, string);
    buff = strtok(buff, ":");

    struct program_token* toReturn = tokenizer_makeGenericToken(buff, PROGTOK__LABEL);

    free(buff);
    return toReturn;
}


struct program_token* tokenizer_makeOpcodeToken(char* string) {
    struct program_token* toReturn = tokenizer_makeGenericToken(string, PROGTOK__INSTRUCTION);

    return toReturn;
}


struct program_token* tokenizer_makeVariableDeclarationToken(char* string) {
    struct program_token* toReturn = tokenizer_makeGenericToken(&(string[4]), PROGTOK__VARIABLE_DEC);
    
    return toReturn;
}

struct program_token* tokenizer_makeGenericToken(char* instruction_text, enum programTokenType tokenType) {
    struct program_token* toReturn = (struct program_token*)calloc(1, sizeof(struct program_token));
    toReturn->prevToken = NULL;
    toReturn->nextToken = NULL;
    toReturn->address = 0;
    toReturn->instruction_text = (char*)calloc(strlen(instruction_text)+1, sizeof(char));
    toReturn->tokenType = tokenType;
    strcpy(toReturn->instruction_text, instruction_text); 

    return toReturn;
}


void tokenizer_printOutToken(struct program_token* t) {
    if(t == NULL)
        return;

    switch(t->tokenType) {

        case PROGTOK__INSTRUCTION:
            printf("[INSTRUCTION token: line#: %d, address in ROM: %d/%4X, text: '%s']\n",t->lineNumber+1,t->romAddress,t->romAddress,t->instruction_text);
            break;

        case PROGTOK__LABEL:
            printf("[LABEL token: line#: %d, address pointed to in ROM: %d/%4X, text: '%s']\n",t->lineNumber+1,t->romAddress,t->romAddress,t->instruction_text);
            break;
        
        case PROGTOK__PREPROC_DIR:
            printf("[PREPROCESSOR DIRECTIVE token: line#: %d, text: '%s']\n",t->lineNumber+1,t->instruction_text);
            break;

        case PROGTOK__VARIABLE_DEC:
             printf("[VARIABLE DECLARATION token: line#: %d, text: '%s']\n",t->lineNumber+1,t->instruction_text);
            break;
        
        default:
            printf("[UNKNOWN token: line#: %d, address in ROM: %d/%4X, text: '%s']\n",t->lineNumber+1,t->address,t->address,t->instruction_text);
            break;
    }
    
}

