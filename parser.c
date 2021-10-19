#include "parser.h"

uint8_t parser(char* text, uint16_t* returnValue) {
    char* token = (char*)calloc(strlen(text)+1,sizeof(char));
    uint16_t val1 = 0, val2 = 0, scratchpad;
    enum parser_state state = PARSERSTATE__INITIAL_NUMBER;
    uint8_t textIndex = 0, tokenIndex = 0;
    while(textIndex < strlen(text)) {
        //1) copy over bytes until a null term or math operator or space or parentheses is found
        while(!parser_internal__isEndOfToken(text[textIndex])) {
            token[tokenIndex++] = text[textIndex++];
        }

        //2) if in a math state (initial or add/sub/mult), perform the action on
        if(tokenIndex > 0) {
            if(parser_getValueOfToken(token, &val2)) {
                printf("\n[Parser]: [FATAL ERROR]: Unable to resolve meaning of \"%s\"",token);
                return 1;
            }
                
            switch(state) {
                case PARSERSTATE__INITIAL_NUMBER:
                    val1 = val2;
                    state = PARSERSTATE__EXPECTING_OPERATOR;
                    break;
                
                case PARSERSTATE__ADD:
                    val1 += val2;
                    state = PARSERSTATE__EXPECTING_OPERATOR;
                    break;
                
                case PARSERSTATE__SUBTRACT:
                    if(val2 > val1) {
                        printf("\n[Parser]: [FATAL ERROR]: Subtraction below zero");
                        return 1;
                    }
                    val1 -= val2;
                    state = PARSERSTATE__EXPECTING_OPERATOR;
                    break;
                
                case PARSERSTATE__MULTIPLY:
                    //TODO
                    break;
                
                case PARSERSTATE__INDEX:
                    //TODO
                    break;

                case PARSERSTATE__EXPECTING_OPERATOR:
                    printf("\n[Parser]: [FATAL ERROR]: Was expecting another operator and got \"%s\".",token);
                    break;
            }

        }

        switch(text[textIndex]) {
            case '+':
                //TODO
                break;
            case '-':
                //TODO
                break;
            case '*':
                //TODO
                break;
            case '[':
                if(library_getVariableAddress(token,&scratchpad)!=LIBRARY_STATUS__NO_ERRORS) {
                    printf("\n[Parser]: [Error]: Indexing attempted on non-variable \"%s\"",token);
                    return 1;
                }

                char* buff = (char*)calloc(strlen(text[textIndex]+1)+1,sizeof(char));
                strcpy(buff, &(text[textIndex+1]));
                buff = strtok(buff, ']');

                textIndex += strlen(buff)+2;//Shift pointer past everything inside the brackets

                if(parser(buff, &val2) != 0) {
                    printf("\n[Parser]: Failed when parsing text between brackets \"%s\"",buff);
                    return 1;
                }

                val1 += val2;
                break;
            case '(':
                //TODO
                break;
            
            case '\0':
                break;
            
            default:
                printf("\n[Parser]: [Error]: Encountered unexpected symbol '%c'",text[textIndex]);
                return 1;
        }

        for(uint8_t i = 0; i < strlen(text); i++) {
            token[i] = 0;
        }
        tokenIndex = 0;
        
    }
   
        

    printf("\n[Parser]: Translated '%s' to: %d/0x%X.",text, val1, val1);

    //    token = strtok(NULL," ");
    //    firstToken = 0;
    //}

//    free(token);

    *returnValue = val1;
    return 0;
}


uint8_t parser_getValueOfToken(char* text, uint16_t* returnValue) {
    //1) Check if the token is a hex value
    if(strlen(text) > 2 && text[0] == '0' && text[1] == 'x') {
        sscanf(text,"%x",(unsigned int*)returnValue);//TODO check for errors
    }

    //2) check if the token is a decimal value
    else if(isdigit(text[0])) {
        sscanf(text, "%d", (int*)returnValue);//TODO check for errors
    }

    //3) check if it's a variable
    else if(library_getVariableAddress(text, returnValue) != LIBRARY_STATUS__NAME_NOT_FOUND);
    
    //4) check if it's a label
    else if(library_getLabelAddress(text, returnValue) != LIBRARY_STATUS__NAME_NOT_FOUND);

    else {
        printf("\n\n[FATAL ERROR]: [Parser]: Unable to resolve meaning of token '%s'",text);
        return 1;
    }
    return 0;
}

uint8_t parser_internal__isEndOfToken(char c) {
   return (c == ' ') || (c == '[') || (c == ']') || (c == '*') || (c == '+') || (c == '-') || (c == '\0');     
}