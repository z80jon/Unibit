#include "parser.h"
#include <inttypes.h>


//===== Local Variables and Functions =====//

enum parser_state {
    PARSERSTATE__INITIAL_VALUE = 0, //< Parser is in its initial state and expecting a new value
    PARSERSTATE__ADD,               //< Parser will subtract next value decoded
    PARSERSTATE__SUBTRACT,          //< Parser will subtract next value decoded
    PARSERSTATE__MULTIPLY,          //< Parser will subtract next value decoded
    PARSERSTATE__INDEX,             //< Parser will index (add value within brackets) to item
    PARSERSTATE__EXPECTING_OPERATOR //< Parser is expecting a mathematical operator
};

/**
 * @brief Returns true if the character passed is one that should delineate the token and indicate
 *        to the parser that the number or variable/define/etc. is ready for interpretation.
 * 
 *        This function is a glorified macro that just identifies end-of-token symbols.
 * 
 * @param c the char to check for the presence of an end-of-token symbol
 * @return true if c is an end-of-token symbol, else false
 */
bool bIsTokenDelimiter(char c);


bool bIsMathTokenDelimiter(char c);


bool bIsWhitespaceDelimiter(char c);


const char* MATH_DELIMITERS = "+-/*()[]";
const char* WHITESPACE_DELIMITERS = "\t\r\n\v\f ";



/**
 * \brief given input text to the parser, separate each number, variable, and operator
 *        into its own element in an array of strings. eg: "1+foo" --> {"1", "+", "foo"}
 * 
 * \param text input text to the parser
 * \return char** a newly allocated and populated array of strings, each containing one
 *                element of the data to be parsed
 */
char** createArrayofTokens(char* text);

/**
 * \brief given a string of text to be input to the parser, count how many discrete elements
 *        that require parsing there are. Examples:
 * 
 *        foo[2] --> 4 (foo, [, 2, ]);  1+(7*3) --> 7 (1,+,(,7,*,3,))
 * 
 * \param text input text
 * \return int number of discrete parseable elements
 */
int getNumberOfTokens(char* text);


uint8_t parser(char* text, uint16_t* returnValue) {
    char* token = (char*)calloc(strlen(text)+1,sizeof(char));
    int val1 = 0, val2 = 0; //Used to store intermediate values to calculate the final value
    int scratchpad;         //Used for temp variables
    int textIndex = 0;      //Index within the text string that the parser is currently parsing
    int tokenIndex = 0;
    
    if(DEBUG_PARSER)printf("\n[Parser]: Started parsing \"%s\".",text);

    //Edge case: Empty input text. Resolve this to zero.
    if(text == NULL || text[0] == '\0') {
        *returnValue = 0;
        return 0;
    }

    //1) separate the text into an array of tokens
    //   (eg, input "1+foo" --> {"1", "+", "foo"})
    char** tokens = createArrayofTokens(text);
    
    while(textIndex < strlen(text)) {

        //1) copy over characters until a null term or math operator or space or parentheses is found
        while(!bIsTokenDelimiter(text[textIndex])) {
            token[tokenIndex++] = text[textIndex++];
        }

        //2) If we didn't just encounter blank text, parse the value we captured
        if(tokenIndex > 0) {
            if(parser__get_value_of_token(token, &val2)) {
                printf("\n[Parser]: [ERROR]: Unable to resolve meaning of \"%s\"",token);
                return 1;
            }

            switch(state) {
                case PARSERSTATE__INITIAL_VALUE:
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

                case PARSERSTATE__EXPECTING_OPERATOR:
                    printf("\n[Parser]: [ERROR]: Was expecting another operator and got \"%s\".",token);
                    break;

                case PARSERSTATE__INDEX:
                    //NYI
                    break;
            }

        }

        //Detect the text at the index
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

            case PARSERSTATE__EXPECTING_OPERATOR:
                //NYI
                break;

            case PARSERSTATE__INDEX:
                //NYI
                break;
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
                    case PARSERSTATE__INITIAL_VALUE:
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

                    case PARSERSTATE__INDEX:
                        //NYI
                        break;

                    case PARSERSTATE__EXPECTING_OPERATOR:
                        //NYI
                        break;

                    default:
                        //NYI
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
    if(DEBUG_PARSER)printf("\n[Parser]: Translated \"%s\" to: %d/0x%X.",text, val1, val1);

    *returnValue = val1;
    return 0;
}


uint8_t parser__get_value_of_token(char* text, uint16_t* returnValue) {
    if(DEBUG_PARSER)printf("\n[parser__get_value_of_token]: Trying to make sense of %s...",text);
    //TODO check sccanf results and make sure we report success/fail accurately

    //Hexadecimal
    if(strlen(text) > 2 && text[0] == '0' && text[1] == 'x') {
        if(DEBUG_PARSER)printf("Decoded to be hexadecimal! Value: ]");
        if(sscanf(&(text[2]),"%x",(unsigned int*) returnValue) != 1) {
            printf("\n[PARSER ERROR: Could not decode hex value]");
            return 1;
        }
            
        if(DEBUG_PARSER)printf("Value: %x",*returnValue);

    //Decimal
    } else if(isdigit(text[0])) {
        if(DEBUG_PARSER)printf("Decoded to be integer! Value: ");
        if(sscanf(text,"%d",(int*) returnValue) != 1) {
            printf("\n[PARSER ERROR: Could not decode integer]");
            return 1;
        }
        if(DEBUG_PARSER)printf("%d",*returnValue);
    } else {
        if(DEBUG_PARSER)printf("Error: Unable to decode!]");
    }

    //Token / pound define
    //else if(library_doesEntryExist(text)) {
    //    *returnValue = library_getEntryValue(text);
    //    return 0;
    //}
    //TODO handle variables and labels gracefully

    return 0;
}


char** createArrayofTokens(char* text) {
    int numTokens = getNumberOfTokens(text);
    char** toReturn = malloc(sizeof(char*) * numTokens);

    for(int i = 0; i < numTokens; i++) {
        //Get the length of text
        
    }
}

int getNumberOfTokens(char* text) {
    int i = 0;
    if(strlen(text) == 0) {
        return 0;
    }
    int numTokens = 1;
    bool textDelim = false;

    while(text[i] != '\0') {
        if(bIsTokenDelimiter(text[i]) && textDelim == false) {
            //If we have encountered a delimiter, it marks the end of the token and the start of another--
            textDelim = true;
            numTokens++;
        } else if(bIsTokenDelimiter(text[i]) && textDelim == true) {
            //If we have not yet reset seeing a delimitor, make sure it's a math operator and not just a
            //repeat white space character before incrementing the counter again
            if(bIsMathTokenDelimiter(text[i]))
                numTokens++;

        } else {
            textDelim = false;
        }
        i++;
    }

    return numTokens;
}


bool bIsTokenDelimiter(char c) {
   return bIsMathTokenDelimiter(c) || bIsWhitespaceDelimiter(c);
}

bool bIsMathTokenDelimiter(char c) {
    return strchr(MATH_DELIMITERS, c) != NULL;
}

bool bIsWhitespaceDelimiter(char c) {
    return strchr(WHITESPACE_DELIMITERS, c) != NULL;
}