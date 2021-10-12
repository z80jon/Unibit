#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "datastructures.h"
#include "fileHandler.h"
#include <string.h>



/**
 * @brief Recursively tokenizes the entire program, doing the following:
 * 
 * -making a discrete program_token for every meaningful part of code (opcode, preprocessor directive, etc)
 * -making dictionary entries where relevant.
 * 
 * @param fp
 * @return struct program_token 
 */
struct program_token* tokenizer_tokenize(char* inputFile);



/**
 * @brief Given a string with a preprocessor directive at the start of it, extract it from the string and attach it to
 *        the end of the current program token, currentToken
 * 
 * @warning if the input string has an invalid token, will halt assembly by callign assembler_haltError() after printing
 *          an error message.
 * 
 * @param string the string containing a preprocessor directive at its start, to be removed.
 * @param currentToken pointer to the current program token (end of linked list)
 * @return char* the input string with the preprocessor token removed
 */
char* tokenizer_makePreprocessorToken(char* string, struct program_token* currentToken);


/**
 * @brief Checks to see if the first word in the string passed is a label or not. Also performs sanity check on label
 *        naming scheme.
 * 
 * @param c the string to check the first word of
 * @return uint8_t 1 if there is a label, else 0.
 */
uint8_t tokenizer_hasLabel(char* c);


#endif