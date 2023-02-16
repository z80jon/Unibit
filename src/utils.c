
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>

const char* MATH_DELIMITERS = "+-/*%%()[]";

const char* WHITESPACE_DELIMITERS = "\t\r\n\v\f ";


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


/**
 * \brief Returns true if the char at text[index] is the first '/' in a '//' comment start
 * 
 * \param text the string to check
 * \param index the index within the string 'text' to check
 * \return true if it is the beginning of a comment
 * \return false if it is not the beginning of a comment
 */
bool bIsStartOfComment(char* text, int index) {
    return (strlen(text) > (index + 1)) &&
            (text[index] == '/' && (text[index+1] == '/' || text[index+1] == '*'));
}

/**
 * \brief Given the index to the beginning of a comment, advance to the end of it
 * 
 * \param text the string that the comment is in
 * \param index the first '/' of the comment
 * \return the index of the first non-comment character, or -1 if the end of the string is reached
 * 
 */
int findEndOfComment(char* text, int index) {
    int i = index+2;

    if(bIsStartOfComment(text, index) == false) {
        printf("ERROR: Input text was not the start of a comment! This is an assembler bug!");
        exit(1);
    }

    //Find the end of it based on if it's '/*' or '//'
    switch(text[i-1]) {
        case '*':
            //Find '*/'
            i+=2;
            while(text[i] != '\0') {
                if(text[i-2] == '*' && text[i-1] == '/') {
                    break;
                }
                i++;
            }
            break;
        case '/':
            //Find end of line
            while(text[i] != '\n' && text[i] != '\0')
                i++;
            if(text[i] == '\n')
                i++;
            break;
    }

    //Return -1 if it's a null terminator
    if(text[i] == '\0')
        return -1;

    return i;
}

