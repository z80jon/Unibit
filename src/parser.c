#include "parser.h"
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <assert.h>

//==== Local Variables and Enums ====//

int numTokens, originalNumTokens;
char** tokens;
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
    eOOO_noOperator = 0,
    eOOO_additionSubtraction,           //< Addition or subtraction (lowest priority)
    eOOO_multiplicationDivisionModulo,  //< Multiplication or division or modulo operator
    eOOO_exponent,                      //< Exponent
    eOOO_parenthesesBracket             //< Parentheses or bracket operator (highest priority)
};

const char* MATH_DELIMITERS = "+-/*%%()[]";
const char* WHITESPACE_DELIMITERS = "\t\r\n\v\f ";


//==== Local Functions ====//
bool bIsTokenDelimiter(char c);
bool bIsMathTokenDelimiter(char c);
bool bIsWhitespaceDelimiter(char c);
enum eOrderOfOperations getPriorityofOperator(char c);
int getValueOfToken(char* text, int32_t* returnValue);
int parse(int startIndex, int32_t* returnValue);
int createArrayofTokens(char* text);
int getNumberOfTokens(char* text);
int removeTokens(int startIndex, int qty);
int findHighestPriorityMathOperator(int startIndex, int* index);
int doMath(int index);



uint8_t parser_begin(char* text, uint16_t* returnValue) {
    int32_t tempVal; //Temporary, larger buffer for return value
    if(DEBUG_PARSER)printf("\n[Parser]: Started parsing \"%s\".",text);
    //Edge case: empty input string
    if(strlen(text) == 0) {
        *returnValue = 0;
        return 0;
    }

    //STEP 1: Turn the text into a discrete array of parsable tokenss
    numTokens = createArrayofTokens(text);
    if(numTokens < 1) {
        printf("\n[Parser]: Unknown error when creating array of tokens");
        *returnValue = 0;
        return 1;
    }


    //STEP 2: Use internal parse function to recursively parse the data
    if(parse(0, &tempVal) != 0) {
        *returnValue = 0;
        if(DEBUG_PARSER)printf("\n[Parser]: Unable to parse input text!");
        removeTokens(0,numTokens);
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
int parse(int startIndex, int32_t* returnValue) {
    int i = startIndex + 1;
    if(DEBUG_PARSER)printf("\n[Parse]: Started parsing at index %d (token text: \"%s\")",startIndex,tokens[startIndex]);

    //STEP 1: BASE CASE: just extract the value of the token IFF...
    //                   a) there is only one token left (AKA we have parsed everything)
    //                   b) the token immediately to our right is a ] or ), indicating that 
    //                      this series of recursive calls is to calculate everything
    //                      inbetween [ ] or ( ).
    if(startIndex == (numTokens-1) || tokens[startIndex+1][0] == ']' || tokens[startIndex+1][0] == ')') {
        if(getValueOfToken(tokens[startIndex],returnValue) != 0) {
            //ERROR STATE
            if(DEBUG_PARSER)printf("\n[PARSER]: Could not interpret value of token \"%s\"!",tokens[startIndex]);
            return 1;
        }
        if(DEBUG_PARSER)printf("\nFinal token value: %d", *returnValue);
        return 0;
    }

    //STEP 2: Find first occurrence of the highest priority math operator
    if(findHighestPriorityMathOperator(startIndex, &i)) {
        //error
        if(DEBUG_PARSER)printf("Could not find highest priority token!");
        return 1;
    }

    if(DEBUG_PARSER)printf("\n[Parse]: highest priority token: \"%s\" (index %d)",tokens[i],i);
    

    //Edge case: user had highest priority token at end of text input
    if(i == (numTokens-1)) {
        if(DEBUG_PARSER)printf("\n[Parser]: ERROR: Highest priority \"%s\" token at end of input string \"%s\"!",tokens[i],parserText);
        return 1;
    }

    //STEP 3: Act on the highest priority math operator
    if(doMath(i)) {
        //Fail
        return 1;
    }

    
    //STEP 5: Recursively parse the rest of the tokens
    return parse(startIndex, returnValue);
}


//=== Parser Token Helper Functiosn ====//

/**
 * @brief Obtains the discrete value of a non-operator token, deciphering it to be a label, address, hex, or dec number,
 *        and returns this value in returnValue
 * 
 * @param text the token text to be parsed
 * @param returnValue pointer to where to store the decoded value of the token text
 * @return int 1 if unable to decode token, else 0.
 */
int getValueOfToken(char* text, int32_t* returnValue) {
    //(DEBUG_PARSER)printf("\n[getValueOfToken]: Trying to make sense of %s...",text);
    //TODO check sccanf results and make sure we report success/fail accurately

    //Hexadecimal
    if(strlen(text) > 2 && text[0] == '0' && text[1] == 'x') {
        //if(DEBUG_PARSER)printf("Decoded to be hexadecimal! Value: ]");
        if(sscanf(&(text[2]),"%x",(unsigned int*) returnValue) != 1) {
            if(DEBUG_PARSER)printf("\n[PARSER ERROR: Could not decode hex value]");
            return 1;
        }

        //if(DEBUG_PARSER)printf("Value: %x",*returnValue);

    //Decimal
    } else if(isdigit(text[0])) {
        //if(DEBUG_PARSER)printf("Decoded to be integer! Value: ");
        if(sscanf(text,"%d",(int*) returnValue) != 1) {
            if(DEBUG_PARSER)printf("\n[PARSER ERROR: Could not decode integer]");
            return 1;
        }
        //if(DEBUG_PARSER)printf("%d",*returnValue);
    } else {
        if(DEBUG_PARSER)printf("\n[PARSER ERROR: Could not decode text \"%s\"]",text);
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
int createArrayofTokens(char* text) {
    int numTokens = getNumberOfTokens(text);
    tokens = malloc(sizeof(char*) * numTokens);
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
        tokens[i] = calloc(endOfToken - startOfToken + 1, sizeof(char));
        memcpy(tokens[i], &(text[startOfToken]), sizeof(char) * (endOfToken - startOfToken));
        //if(DEBUG_PARSER)printf("\n[createArrayOfTokens]: created token with text \"%s\"",array[0][i]);

        startOfToken = endOfToken;
        
        //Before moving on to the next token (if applicable), advance past any whitespace (if applicable)
        while(bIsWhitespaceDelimiter(text[startOfToken]))
            startOfToken++;
        endOfToken = startOfToken + 1;
    }
    
    return numTokens;
}


/**
 * \brief Utility to remove a given number of tokens from the array of tokens being parsed, then
 *        shifts down the tokens that follow.
 * 
 * \note updates numTokens
 * \note modifies tokens[], and reallocates it based on the number of tokens removed
 * 
 * \param startIndex the first index in the global tokens[] array to remove
 * \param qty the number of tokens, including and following tokens[startIndex], to remove
 * \return int 0 if successful, else 1
 */
int removeTokens(int startIndex, int qty) {
    assert((startIndex+qty-1) < numTokens);
    assert(qty > 0);
    assert(startIndex >= 0);
    if(qty == 0) return 0;

    //1) Free tokens to be overwritten
    for(int i = 0; i < qty; i++) {
        free(tokens[startIndex+i]);
    }

    //2) move down tokens
    for(int i = startIndex+qty; i < numTokens; i++) {
        tokens[i-qty] = tokens[i];
    }

    //3) update numTokens and reallocate the array size
    numTokens -= qty;
    tokens = realloc(tokens, numTokens*sizeof(char*));
    return 0;
}


/**
 * \brief Searches the array of tokens, starting from startIndex, and returns the highest priority
 *        math operator (using PEMDAS).
 * 
 * \param startIndex the index in the array of tokens to start at
 * \param index pointer to an integer that the highest priority math operator will be stored
 *              in if found.
 * \return int 0 if successful, else 1
 */
int findHighestPriorityMathOperator(int startIndex, int* index) {
    int i = startIndex;
    
    while(i < numTokens && tokens[i][0] != ']' && tokens[i][0] != ')') {
        if(getPriorityofOperator(tokens[i][0]) > getPriorityofOperator(tokens[*index][0])) {
                *index = i;
        }
        i++;
    }

    //If we didn't find any math operators, return (fail)
    if(!bIsMathTokenDelimiter(tokens[*index][0]))
        return 1;

    return 0;
}



//==== Math Helper Functions ====//

/**
 * \brief Given an index of a math operator, parentheses, or bracket, performs the operator
 *        (as applicable) on the adjacent indices in the array of Tokens.
 * 
 * \param index 
 * \return int 
 */
int doMath(int index) {
    int32_t scratchPad1, scratchPad2; //for return value usage
    if((index == 0 || index == numTokens-1) &&
        tokens[index][0] != '(' && tokens[index][0] != '[') {
        if(DEBUG_PARSER)printf("\n[doMath]: ERROR: illegal index %d!",index);
        return 1;
    }
    if(tokens[index][0] == '[' && index >= numTokens-2) {
        if(DEBUG_PARSER)printf("\n[doMath]: ERROR: Opening '[' with no closing ']'!");
        return 1;
    }
    
    //Step 1: Get the value of each token surrounding the index, IFF it's not a ( or [
    if(tokens[index][0] != '(') {
        if(getValueOfToken(tokens[index-1],&scratchPad1)) {
            if(DEBUG_PARSER)printf("\n[Parser]: Could not decode first operand \"%s\"!",tokens[index-1]);
            return 1;
        }
        if(tokens[index][0] != '[' && getValueOfToken(tokens[index+1],&scratchPad2)) {
            if(DEBUG_PARSER)printf("\n[Parser]: Could not decode second operand \"%s\"!",tokens[index+1]);
            return 1;
        }

        if(DEBUG_PARSER)printf("\n[doMath]: Performing math equation %d %c %d", scratchPad1, tokens[index][0], scratchPad2);
    }


    //Step 2: Act on the math operator
    switch(tokens[index][0]) {
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

        case '[':
            if(parse(index+1, &scratchPad2)) {
                if(DEBUG_PARSER)printf("\n[Parser]: Error occurred while parsing recursively!");
                return 1;
            }
            break;

        case '(':
            if(parse(index+1, &scratchPad2)) {
                if(DEBUG_PARSER)printf("\n[Parser]: Error occurred while parsing recursively!");
                return 1;
            }
            break;

        default:
            //THIS SHOULD NEVER EXECUTE
            if(DEBUG_PARSER)printf("\n[Parser]: ERROR: Bad math operator at index %d, equation of \"%s %c %s\"!",index, tokens[index-1], tokens[index][0], tokens[index+1]);
            if(DEBUG_PARSER)printf("This is most likely an assembler bug!");
            return 1;
    }

    //Step 3: Cleanup and remove now-redundant tokens
    switch(tokens[index][0]) {

        case '[':
            //Remove opening bracket, solution, and closing bracket
            removeTokens(index, 3); 
            //Replace variable/define being indexed with its value + the index offset
            tokens[index-1] = realloc(tokens[index-1], 10*sizeof(char));
            sprintf(tokens[index-1],"%d",scratchPad1+scratchPad2);
            break;

        case '(':
            //Remove '('
            removeTokens(index, 1);
            //Remove ')' (now only index + 1 due to removal of '(')
            removeTokens(index+1, 1);
            break;

        default:
            //Store the answer where the first operand was and delete the operator and second operand
            tokens[index-1] = realloc(tokens[index-1], 10*sizeof(char));
            sprintf(tokens[index-1],"%d",scratchPad1);
            removeTokens(index, 2);
            break;
    }

    return 0;
}



//==== Text Helper Functions ====//

/**
 * \brief given a string of text to be input to the parser, count how many discrete elements
 *        that require parsing there are. Examples:
 * 
 *        foo[2] --> 4 (foo, [, 2, ]);  1+(7*3) --> 7 (1,+,(,7,*,3,))
 * 
 * \param text input text
 * \return int number of discrete parseable elements
 */
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


/**
 * @brief Returns true if the character passed is one that should delineate the token and indicate
 *        to the parser that the number or variable/define/etc. is ready for interpretation.
 * 
 *        This function is a glorified macro that just identifies end-of-token symbols.
 * 
 * @param c the char to check for the presence of an end-of-token symbol
 * @return true if c is an end-of-token symbol, else false
 */
bool bIsTokenDelimiter(char c) {
   return bIsMathTokenDelimiter(c) || bIsWhitespaceDelimiter(c);
}


/**
 * \brief Returns true if the character passed is found in MATH_DELIMITERS. In other words,
 *        it checks if it is a math operator (eg, +, -, *, etc) OR a [, ], (, or).
 * 
 * \param c the character to check
 * \return true if it is a math operator or parentheses/bracket
 * \return false if not
 */
bool bIsMathTokenDelimiter(char c) {
    return strchr(MATH_DELIMITERS, c) != NULL;
}


/**
 * \brief Returns true if the character passed is found in WHITESPACE_DELIMITERS. In
 *        other words, it checks if it is a space, tab, newline, carriage return, etc.
 * 
 * \param c the character to check
 * \return true if it is some form of whitespace, carriage return, etc.
 * \return false if not
 */
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
        return eOOO_noOperator;
    }
}


