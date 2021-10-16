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
        else {
            sscanf(token, "%d", &current_value);
        }

    //    token = strtok(NULL," ");
    //    firstToken = 0;
    //}
    return current_value;
}