#include "parser.h"

uint16_t parser(char* text) {
    char* token = text;
    //token = strtok(text," ");
    uint16_t current_value = 0;
    //uint8_t firstToken = 1;
    //uint8_t nextOperator;
    //while(token != NULL) {
        
   
        //1) Check if the token is a hex value
        if(strlen(token) > 2 && token[0] == '0' && token[1] == 'x') {
            sscanf(token,"%x",&current_value);
        }

        //2) check if the token is a decimal value
        else if(isdigit(token[0])) {
            sscanf(token, "%d", (int*)&current_value);
        }

        //3) check if it's a variable
        else if(library_getVariableAddress(token, &current_value) != LIBRARY_STATUS__NAME_NOT_FOUND);
        
        //4) check if it's a label
        else if(library_getLabelAddress(token, &current_value) != LIBRARY_STATUS__NAME_NOT_FOUND);

        else {
            printf("\n\n[FATAL ERROR]: [Parser]: Unable to resolve meaning of token '%s'",token);
        }

        printf("\n[Parser]: [DEBUG]: Received string '%s' as input. Interpreted to have value of: %d / 0x%X.",token, current_value, current_value);

    //    token = strtok(NULL," ");
    //    firstToken = 0;
    //}
    return (uint16_t)current_value;
}