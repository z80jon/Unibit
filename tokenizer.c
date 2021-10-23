#include "tokenizer.h"


struct program_token* tokenizer_tokenize(char* inputFile) {

    uint32_t numLines;
    char** linesOfText;
    fileHandler__read_in_file(inputFile,&linesOfText, &numLines);

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
        char* sterilizedLineText = fileHandler__sterilize_text(linesOfText[currentLine]);//remove any comments, extra spaces, etc.
    //TODO record line number
        uint8_t lineHadLabel = 0;
        if(sterilizedLineText != NULL) {


            if(tokenizer__has_label(sterilizedLineText)) {
                //printf("\nLine %d: \"%s\" --> Label",currentLine+1,sterilizedLineText);
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
                //printf("\nLine %d: \"%s\" --> Preprocessor Directive",currentLine+1,sterilizedLineText);
                next = tokenizer_makePreprocessorToken(sterilizedLineText);

            } else if(tokenizer__has_opcode(sterilizedLineText)) {
                //printf("\nLine %d: \"%s\" --> OpCode",currentLine+1,sterilizedLineText);
                next = tokenizer_makeOpcodeToken(sterilizedLineText);
                next->romAddress = romAddress++;//we only assign ROM addresses to opcodes
                
            } else if(tokenizer__has_variable(sterilizedLineText)) {
                //printf("\nLine %d: \"%s\" --> Variable",currentLine+1,sterilizedLineText);
                next = tokenizer_makeVariableDeclarationToken(sterilizedLineText);
                
            } else if(!lineHadLabel){
                //printf("\n[Tokenizer]: [ERROR]: Unable to determine what type of token '%s' is!",sterilizedLineText);
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

    //Due to a design flaw of the code, the 'head' is never actually used and contains garbage data. This correct that by
    //removing it.
    head = head->nextToken;
    free(head->prevToken);
    head->prevToken = NULL;
    return head;
}


uint8_t tokenizer__has_label(char* c) {
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


uint8_t tokenizer__has_opcode(char* c) {
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


uint8_t tokenizer__has_variable(char* c) {
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

    char* buff = (char*)calloc(strlen(string)+1,sizeof(char));
    strcpy(buff,string);
    buff = strtok(buff, " ");
    for(uint8_t i = 0; i < LOAD_STRINGS_LENGTH; i++)
        if(strcmp(buff,LOAD_STRINGS[i]) == 0)
            toReturn->opcode = LOAD;
    for(uint8_t i = 0; i < NEGATE_STRINGS_LENGTH; i++)
        if(strcmp(buff, NEGATE_STRINGS[i])==0)
            toReturn->opcode = NEGATE;
    for(uint8_t i = 0; i < STORE_STRINGS_LENGTH; i++)
        if(strcmp(buff, STORE_STRINGS[i])==0)
            toReturn->opcode = STORE;
    for(uint8_t i = 0; i < JUMP_STRINGS_LENGTH; i++)
        if(strcmp(buff, JUMP_STRINGS[i])==0)
            toReturn->opcode = JUMPIFZERO;
    if(toReturn->opcode == UNDEFINED) {
        printf("\n[Tokenizer]: [Error]: Unable to understand opcode token \"%s\". This is likely a bug in the assembler!",buff);
        return NULL;
    }
    
    //printf("\nBuff is currently: \"%s\"",buff);
    buff = strtok(NULL, " ");
    if(buff == NULL) {
        toReturn->instruction_text = NULL;
        return toReturn;
    }
    //printf(", and is now: \"%s\"",buff);
    free(toReturn->instruction_text);
    toReturn->instruction_text = (char*)calloc(strlen(buff)+1, sizeof(char));
    strcpy(toReturn->instruction_text, buff);

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
    toReturn->romData = 0;
    toReturn->instruction_text = (char*)calloc(strlen(instruction_text)+1, sizeof(char));
    toReturn->tokenType = tokenType;
    strcpy(toReturn->instruction_text, instruction_text); 

    return toReturn;
}


void tokenizer__print_out_token(struct program_token* t) {
    if(t == NULL)
        return;

    switch(t->tokenType) {

        case PROGTOK__INSTRUCTION:
            printf("[INSTRUCTION token: line#: %d, address in ROM: %d/%4X, ",t->lineNumber+1,t->romAddress,t->romAddress);
            
            switch(t->opcode) {
                case LOAD:
                    printf("LOAD from %d/0x%X, ",t->romData & 0x3FFF, t->romData & 0x3FFF);
                    break;
                case NEGATE:
                    printf("NEGATE (with dangling address %d/0x%X), ",t->romData & 0x3FFF, t->romData & 0x3FFF);
                    break;
                case STORE:
                    printf("STORE to %d/0x%X, ",t->romData & 0x3FFF, t->romData & 0x3FFF);
                    break;
                case JUMPIFZERO:
                    printf("JUMP to %d/0x%X, ",t->romData & 0x3FFF , t->romData & 0x3FFF);
                    break;
            }
            printf("text: '%s']\n",t->instruction_text);
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
            printf("[UNKNOWN token: line#: %d, address in ROM: %d/%4X, text: '%s']\n",t->lineNumber+1,t->romData,t->romData,t->instruction_text);
            break;
    }
    
}


struct program_token* tokenizer__remove_token_from_chain(struct program_token* token) {
    if(token->instruction_text != NULL)
        free(token->instruction_text);
    
    if(token->prevToken != NULL)
        token->prevToken->nextToken = token->nextToken;
    if(token->nextToken != NULL)
        token->nextToken->prevToken = token->prevToken;

    struct program_token* toReturn = token->nextToken;
    free(token);
    return toReturn;
}