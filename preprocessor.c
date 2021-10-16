#include "preprocessor.h"



uint8_t preprocessor_run(struct program_token* head) {
    struct program_token* token = head;

    while(token != NULL) {

        //Variable: enter it into library and remove from the chain
        if(token->tokenType == PROGTOK__VARIABLE_DEC) {
            //Variable syntax:
            //var <name> <bits> <address_to_store_in_RAM(optional)>
            
            //TODO add variable to library
            //TODO remove variable token from linked list
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

}