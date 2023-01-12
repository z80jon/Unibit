#include "parser.h"
#include <inttypes.h>
#include <limits.h>
#include <math.h>

//===== Local Variables and Functions =====//

int numTokens, originalNumTokens;
char* parserText; //< Is set to whatever the current input text being parsed is for error printing purposes

enum parser_state {
    PARSERSTATE__INITIAL_VALUE = 0, //< Parser is in its initial state and expecting a new value
    PARSERSTATE__ADD,               //< Parser will subtract next value decoded
    PARSERSTATE__SUBTRACT,          //< Parser will subtract next value decoded
    PARSERSTATE__MULTIPLY,          //< Parser will subtract next value decoded
    PARSERSTATE__INDEX,             //< Parser will index (add value within brackets) to item
    PARSERSTATE__EXPECTING_OPERATOR //< Parser is expecting a mathematical operator
};

enum eOrderOfOperations {
    eOOO_additionSubtraction = 0,       //< Addition or subtraction (lowest priority)
    eOOO_multiplicationDivisionModulo,  //< Multiplication or division or modulo operator
    eOOO_exponent,                      //< Exponent
    eOOO_parenthesesBracket             //< Parentheses or bracket operator (highest priority)
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


enum eOrderOfOperations getPriorityofOperator(char c);

//TODO look into use of atoi/itoa

/**
 * @brief Obtains the discrete value of a non-operator token, deciphering it to be a label, address, hex, or dec number,
 *        and returns this value in returnValue
 * 
 * @param text the token text to be parsed
 * @param returnValue pointer to where to store the decoded value of the token text
 * @return int 1 if unable to decode token, else 0.
 */
int getValueOfToken(char* text, int32_t* returnValue);

/**
 * \brief Given an array of tokens to parse, returns the value
 * 

 * 
 * \param tokens 
 * \param returnValue 
 * \return uint8_t 
 */


/**
 * \brief Recursively parses a series of tokens into a 16-bit unsigned integer, starting from
 *        startIndex.
 * 
 * 
 * 1) Check for base case (if no tokens beyond startIndex, populate returnValue with token's value)
 * 2) Find highest priority operator beyond startIndex
 * --If ( or [  found, call parse with them as the index and continue on
 * 3) Act on the highest priority math operator
 * 4) Remove tokens we have operated on
 * 5) call parse again with same start index
 * 
 * \param tokens the array of token strings to parse
 * \param startIndex the index this call of parse() should start at
 * \param returnValue 16-bit value that will be populated with the final value of char** tokens
 * \return int 0 if successful, else 1
 */
int parse(char** tokens, int startIndex, int32_t* returnValue);


/**
 * \brief Given a non-empty text input, isolates each discrete parsable token (variables,
 *        numbers, math operators, etc) into its own whitespace-free entry in an array.
 * 
 *        eg: "1 + foo" --> {"1", "+", "foo"}
 * 
 * \param text the input text to split into discrete entries in the array
 * \param array the array to be populated
 * \return int number of elements in the array, or -1 if an error occurs
 */
int createArrayofTokens(char* text, char*** array);


const char* MATH_DELIMITERS = "+-/*%%()[]";
const char* WHITESPACE_DELIMITERS = "\t\r\n\v\f ";


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


uint8_t parser_begin(char* text, uint16_t* returnValue) {
    int32_t tempVal; //Temporary, larger buffer for return value
    if(DEBUG_PARSER)printf("\n[Parser]: Started parsing \"%s\".",text);
    //Edge case: empty input string
    if(strlen(text) == 0) {
        *returnValue = 0;
        return 0;
    }

    //STEP 1: Turn the text into a discrete array of parsable tokenss
    char** tokens = NULL;
    numTokens = createArrayofTokens(text, &tokens);
    if(numTokens < 1) {
        printf("\n[Parser]: Unknown error when creating array of tokens");
        *returnValue = 0;
        return 1;
    }


    //STEP 2: Use internal parse function to recursively parse the data
    if(parse(tokens, 0, &tempVal) != 0) {
        *returnValue = 0;
        printf("\n[Parser]: Unable to parse input text!");
        for(int i = 0; i < numTokens; i++)
            free(tokens[i]);
        free(tokens);
        return 1;
    }
    
    //Free remaining allocated memory
    free(tokens[0]);
    free(tokens);

    if(tempVal > UINT16_MAX || tempVal < 0) {
        printf("\n[Parser]: Input text \"%s\" Did not fit into uint16! Equated to %d!",text,tempVal);
        *returnValue = 0;
        return 1;
    }
    
    *returnValue = (uint16_t) tempVal;
    //if(DEBUG_PARSER)printf("\n[Parser]: Translated \"%s\" to: %d/0x%X.",text, *returnValue, *returnValue);
    return 0;
}


int parse(char** tokens, int startIndex, int32_t* returnValue) {
    int i = startIndex + 1;
    int hpi = i; //index of first highest priority math operator
    int32_t scratchPad1, scratchPad2; //for return value usage
    if(DEBUG_PARSER)printf("\n[parse]: Started parsing at index %d (token text: \"%s\")",startIndex,tokens[startIndex]);

    //STEP 1: BASE CASE: just extract the value of the token IFF...
    //                   a) there is only one token left (AKA we have parsed everything)
    //                   b) the token immediately to our right is a ] or ), indicating that 
    //                      this series of recursive calls is to calculate everything
    //                      inbetween [ ] or ( ).
    if(startIndex == (numTokens-1) || tokens[startIndex+1][0] == ']' || tokens[startIndex+1][0] == ')') {
        if(getValueOfToken(tokens[startIndex],returnValue) != 0) {
            //ERROR STATE
            printf("\n[PARSER]: Could not interpret value of token \"%s\"!",tokens[startIndex]);
            return 1;
        }
        if(DEBUG_PARSER)printf("\nFinal token value: %d", *returnValue);


        //Remove the ending ] or ) if applicable
        if(startIndex < (numTokens-1) && (tokens[startIndex+1][0] == ']' || tokens[startIndex+1][0] == ')')) {
            free(tokens[startIndex+1]);
            for(int j = startIndex+2; j < numTokens; j++) {
                tokens[j-1] = tokens[j];
            }
            numTokens--;
        }

        return 0;
    }

    //STEP 2: Find first occurrence of the highest priority math operator
    while(i < numTokens && tokens[i][0] != ']' && tokens[i][0] != ')') {
        if(bIsMathTokenDelimiter(tokens[i][0])) {
            //If it's an opening or closing bracket, recursively call the parser. If it's an
            //index operator, change the '[' to a '+' and bypass all other operations,
            //but if it's a '(', replace the '(' with the value directly
            if((tokens[i][0] == '[') || (tokens[i][0] == '(')) {
                parse(tokens, i+1, &scratchPad1);

                if(tokens[i][0] == '[') {
                    realloc(tokens[i+1],sizeof(char) * 16);
                    if(snprintf(tokens[i+1],sizeof(char)*15,"%d",scratchPad1) > 14) {
                        //Error -- wrote too many characters
                    }

                    //Since indexing effectively adds the contents between the [ ] to the
                    //variable or number before it, change the [ to a + to let the existing
                    //addition logic handle it
                    tokens[i][0] = '+';

                    //TODO add sanity check to make sure we're not indexing a number
                    
                    //Break out of the loop and force the parser to act on this data first
                    break;

                } else if((tokens[i][0] == '(')) {
                    realloc(tokens[i],sizeof(char) * 16);
                    if(snprintf(tokens[i],sizeof(char)*15,"%d",scratchPad1) > 14) {
                        //Error -- wrote too many characters
                    }

                }

                //Copy over the result
                free(tokens[i][0]);
                tokens[i] = tokens[i+1];

            //Check to see if it's higher priority than our existing operator
            } else if(getPriorityofOperator(tokens[i][0]) > getPriorityofOperator(tokens[hpi][0])) {
                hpi = i;
            }
        }
        i++;
    }

    //Edge case: user had highest priority token at end of text input
    if(hpi == (numTokens-1)) {
        printf("\n[Parser]: ERROR: Highest priority \"%s\" token at end of input string \"%s\"!",tokens[hpi],parserText);
        return 1;
    }

    //STEP 3: Act on the highest priority math operator
    getValueOfToken(tokens[hpi-1],&scratchPad1);
    getValueOfToken(tokens[hpi+1],&scratchPad2);
    switch(tokens[hpi][0]) {
        case '^':
            scratchPad1 = pow(scratchPad1, scratchPad2);
            break;
        
        case '*':
            scratchPad1 = scratchPad1 * scratchPad2;
            break;

        case '/':
            //First, check to see if there will be remainders
            if((scratchPad1 % scratchPad2)!= 0) {
                //TODO handle error
            }
            scratchPad1 = scratchPad1 / scratchPad2;
            break;

        case '%':
            scratchPad1 = scratchPad1 % scratchPad2;
            break;
        
        case '+':
            scratchPad1 = scratchPad1 + scratchPad2;
            break;
        
        case '-':
            scratchPad1 = scratchPad1 - scratchPad2;
            break;
    }

    //STEP 4: free tokens operated on, replace first operand with result (in text)
    //Free up unneeded strings...
    free(tokens[hpi-1]);
    free(tokens[hpi]);
    free(tokens[hpi+1]);

    //Make a new one with the result...
    char* newToken = calloc(10, sizeof(char));
    sprintf(newToken,"%d",scratchPad1);
    tokens[hpi-1] = newToken;

    //Shift down any remaining tokens
    for(int j = hpi+1; j < numTokens; j++) {
        tokens[j-1] = tokens[j];
    }

    //Update the number of tokens and shrink the array accordingly.
    numTokens -= 2;
    realloc(tokens, sizeof(char*) * numTokens);
    
    //STEP 5: Recursively parse the rest of the tokens
    return parse(tokens, startIndex, returnValue);
}


int getValueOfToken(char* text, int32_t* returnValue) {
    if(DEBUG_PARSER)printf("\n[getValueOfToken]: Trying to make sense of %s...",text);
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
        return 1;
    }

    //Token / pound define
    //else if(library_doesEntryExist(text)) {
    //    *returnValue = library_getEntryValue(text);
    //    return 0;
    //}
    //TODO handle variables and labels gracefully

    return 0;
}


int createArrayofTokens(char* text, char*** array) {
    int numTokens = getNumberOfTokens(text);
    array[0] = malloc(sizeof(char*) * numTokens);
    int startOfToken = 0;
    int endOfToken   = 0;
    
    //advance the start index to meaningful data if it isn't there already
    while(bIsWhitespaceDelimiter(text[startOfToken]))
        startOfToken++;
    endOfToken = startOfToken;

    //Populate each token's spot in the array with only the token and no whitespace
    for(int i = 0; i < numTokens; i++) {

        //Find the length (sans whitespace) of the token
        if(bIsMathTokenDelimiter(text[startOfToken])) {
            endOfToken = startOfToken + 1;
        } else {
            while(bIsTokenDelimiter(text[endOfToken]) == false) {
                endOfToken++;
            }
        }
        

        //Copy over the text to a new string
        array[0][i] = calloc(endOfToken - startOfToken + 1, sizeof(char));
        memcpy(array[0][i], &(text[startOfToken]), sizeof(char) * (endOfToken - startOfToken));
        //if(DEBUG_PARSER)printf("\n[createArrayOfTokens]: created token with text \"%s\"",array[0][i]);

        startOfToken = endOfToken;
        
        //Before moving on to the next token (if applicable), advance past any whitespace (if applicable)
        while(bIsWhitespaceDelimiter(text[startOfToken]))
            startOfToken++;
        endOfToken = startOfToken + 1;
    }
    
    return numTokens;
}


int getNumberOfTokens(char* text) {
    int i = 1;
    if(strlen(text) == 0) {
        return 0;
    }
    int numTokens = 1;
    bool textDelim = true;

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

        } else if(!bIsTokenDelimiter(text[i]) && bIsTokenDelimiter(text[i-1])) {
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


enum eOrderOfOperations getPriorityofOperator(char c) {
    switch(c) {
        case '+':
        case '-':
        return eOOO_additionSubtraction;

        case '*':
        case '/':
        case '%':
        return eOOO_multiplicationDivisionModulo;

        case '^':
        return eOOO_exponent;

        case '(':
        case '[':
        return eOOO_parenthesesBracket;

        //In case we are ever passed a non-math operator, return -1
        default:
        return -1;
    }
}