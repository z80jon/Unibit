#include "preprocessor.h"



uint8_t preprocessor_run(struct program_token* head) {
    printf("\n[Preprocessor]: Starting");
    struct program_token* token = head;

    while(token != NULL) {

        //Variable: enter it into library and remove from the chain
        if(token->tokenType == PROGTOK__VARIABLE_DEC) {
            //Variable syntax:
            //var <name> <bits> <address_to_store_in_RAM(optional)>
            
            //NOTE: strtok WILL modify the string in the token, but that is OK as we are deleting the token anyways.
            char* strtok_token = strtok(token->instruction_text, " ");

            if(strtok_token == NULL) {
                printf("\n\n[FATAL ERROR]: [Preprocessor]: Variable name not specified on line %d", token->lineNumber);
            }
            char* varName = (char*)calloc(strlen(strtok_token),sizeof(char));
            strcpy(varName, strtok_token);

            strtok_token = strtok(NULL, " ");
            uint8_t errorCode;
            if(strtok_token == NULL) {//Case: var <name> --> assume size is one bit.
                errorCode = library_addVariable(varName, 1);

            } else {
                uint16_t numBits = parser(strtok_token);
                strtok_token = strtok(NULL, " ");

                if(strtok_token == NULL) {//Case: var <name> <numBits>
                    errorCode = library_addVariable(varName, numBits);

                } else {//Case: var <name> <numBits> <address>
                    uint16_t addr = parser(strtok_token);
                    errorCode = library_addVariableWithAddress(varName, numBits, addr);
                }
            }

            switch(errorCode) {
                case LIBRARY_STATUS__NAME_EXISTS:
                    printf("\n\n[FATAL ERROR]: [Preprocessor]: Variable '%s' (line %d) is was declared elsewhere!",varName, token->lineNumber+1);
                    return 1;
                
                case LIBRARY_STATUS__SYNTAX_ERROR:
                    printf("\n\n[FATAL ERROR]: [Preprocessor]: Variable '%s' (line %d) does not pass syntax check!",varName, token->lineNumber+1);
                    return 1;
                
                case LIBRARY_STATUS__ADDR_CONFLICT:
                    printf("\n\n[FATAL ERROR]: [Preprocessor]: Variable '%s' (line %d) overlaps with another variable's address space!",varName, token->lineNumber+1);
                    return 1;

            }

            free(varName);
            free(token->instruction_text);

            token->prevToken->nextToken = token->nextToken;
            token->nextToken->prevToken = token->prevToken;
            free(token);
        }

        if(token->tokenType == PROGTOK__PREPROC_DIR) {
            //Preprocessor syntaxes supported:
            //.include "filepath.txt" //inserts the file filepath.txt inline with the rest of the code

            //TODO handle
        }

        //Label: enter it into library, but don't remove from chain until able to resolve to a ROM address.
        if(token->tokenType == PROGTOK__LABEL) {
            uint8_t errorCode = library_addLabel(token->instruction_text);
            if(errorCode == LIBRARY_STATUS__NAME_EXISTS) {
                printf("\n\n[FATAL ERROR]: [Preprocessor]: On line %d, label '%s' was defined but is already in use!",token->lineNumber,token->instruction_text);
                return 1;
            }
        }

        token = token->nextToken;
    }

    return 0;
}