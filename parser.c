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
                printf("\n[Parser]: [ERROR]: Unable to resolve meaning of \"%s\"",token);
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
                        printf("\n[Parser]: [ERROR]: Subtraction below zero");
                        return 1;
                    }
                    val1 -= val2;
                    state = PARSERSTATE__EXPECTING_OPERATOR;
                    break;
                
                case PARSERSTATE__MULTIPLY:
                    val1 *= val2;
                    break;
                
                case PARSERSTATE__INDEX:
                    //TODO
                    break;

                case PARSERSTATE__EXPECTING_OPERATOR:
                    printf("\n[Parser]: [ERROR]: Was expecting another operator and got \"%s\".",token);
                    break;
            }

        }

        switch(text[textIndex]) {
            case '+':
                state = PARSERSTATE__ADD;
                break;

            case '-':
                state = PARSERSTATE__SUBTRACT;
                break;

            case '*':
                state = PARSERSTATE__MULTIPLY;
                break;

            case '[':
                if(library_getVariableAddress(token,&scratchpad)!=LIBRARY_STATUS__NO_ERRORS) {
                    printf("\n[Parser]: [ERROR]: Indexing attempted on non-variable \"%s\"",token);
                    return 1;
                }

                textIndex++;//Advance past the first '('

                //Find the index of the matching ] and set it to null terminator
                uint8_t matching_bracket_index;
                uint8_t closing_brackets_remaining = 1;
                for(matching_bracket_index = textIndex; matching_bracket_index < strlen(text) && closing_brackets_remaining > 0; matching_bracket_index++) {
                    if(text[matching_bracket_index] == ']')
                        closing_brackets_remaining--;
                    if(text[matching_bracket_index] == '[')
                        closing_brackets_remaining++;
                }
                matching_bracket_index--;//Because FOR increments before checking, matching_bracket_index will overshoot the matching ] by 1.
                if(text[matching_bracket_index] == '\0') {
                    printf("\n[Parser]: [ERROR]: Ran into [ with no matching ] when parsing text \"%s\"",text);
                    return 1;
                }
                text[matching_bracket_index] = '\0';

                //Copy over the text within the matching [ ]
                char* buff = (char*)calloc(strlen(&(text[textIndex])),sizeof(char));
                strcpy(buff, &(text[textIndex]));

                text[matching_bracket_index] = ']';//Putting it back to this will make debug outputs look a bit cleaner.

                textIndex = matching_bracket_index;//Shift pointer to closing bracket
                if(parser(buff, &val2) != 0) {
                    printf("\n[Parser]: [ERROR]: Failed when parsing text between brackets \"%s\"",buff);
                    return 1;
                }

                val1 += val2;
                state = PARSERSTATE__EXPECTING_OPERATOR;
                break;

            case '(':
                textIndex++;//Advance past the first '('

                //Find the index of the matching ) and set it to null terminator
                uint8_t matching_parentheses_index;
                uint8_t closing_parentheses_remaining = 1;
                for(matching_parentheses_index = textIndex; matching_parentheses_index < strlen(text) && closing_parentheses_remaining > 0; matching_parentheses_index++) {
                    if(text[matching_parentheses_index] == ')')
                        closing_parentheses_remaining--;
                    if(text[matching_parentheses_index] == '(')
                        closing_parentheses_remaining++;
                }

                matching_parentheses_index--;//Because FOR increments before checking, matching_parentheses_index will overshoot the matching ] by 1.
                if(text[matching_parentheses_index] == '\0') {
                    printf("\n[Parser]: [ERROR]: Ran into ( with no matching ) when parsing text \"%s\"",text);
                    return 1;
                }
                text[matching_parentheses_index] = '\0';

                //Copy over the text within the matching [ ]
                buff = (char*)calloc(strlen(&(text[textIndex])),sizeof(char));
                strcpy(buff, &(text[textIndex]));

                text[matching_parentheses_index] = ')';//Putting it back to this will make debug outputs look a bit cleaner.

                textIndex = matching_parentheses_index;//Shift pointer to closing parentheses

                if(parser(buff, &val2) != 0) {
                    printf("\n[Parser]: [ERROR]: Failed when parsing text between parentheses \"%s\"",buff);
                    return 1;
                }

                switch(state) {
                    case PARSERSTATE__INITIAL_NUMBER:
                        val1 = val2;
                        break;
                    case PARSERSTATE__ADD:
                        val1 += val2;
                        break;
                    case PARSERSTATE__MULTIPLY:
                        printf("Multiplying\n");
                        val1 *= val2;
                        break;
                    case PARSERSTATE__SUBTRACT:
                        if(val2 > val1) {
                        printf("\n[Parser]: [ERROR]: Subtraction below zero");
                        return 1;
                        }
                        val1 -= val2;
                        break;
                }
                state = PARSERSTATE__EXPECTING_OPERATOR;
                break;
            
            case '\0':
                break;
            
            default:
                printf("\n[Parser]: [ERROR]: Encountered unexpected symbol '%c'",text[textIndex]);
                return 1;
        }

        for(uint8_t i = 0; i < strlen(text); i++) {
            token[i] = 0;
        }
        tokenIndex = 0;
        textIndex++;
        
    }
   
    free(token);
    printf("\n[Parser]: Translated \"%s\" to: %d/0x%X.",text, val1, val1);

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
        printf("\n[Parser]: [ERROR]: Unable to resolve meaning of token '%s'",text);
        return 1;
    }
    return 0;
}

uint8_t parser_internal__isEndOfToken(char c) {
   return (c == ' ') || (c == '[') || (c == ']') || (c == '*') || (c == '+') || (c == '-') || (c == '\0') || (c=='(') || (c==')');
}