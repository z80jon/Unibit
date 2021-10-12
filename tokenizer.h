#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "datastructures.h"
#include "fileHandler.h"
#include <string.h>



static const char* OPCODE_STRINGS[] = {"l ", "load", "s", "store", "negate", "n", "jumpifzero", "jiz"};
#define OPCODE_STRINGS_LENGTH 8


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
 * @brief Given a string with a preprocessor directive in it, create a new token containing the directive and return it.
 * 
 * @param string the string with the preprocessor directive
 * @return struct program_token* a pointer to a new calloc'd program_token configured as a preprocessor token
 */
struct program_token* tokenizer_makePreprocessorToken(char* string);


/**
 * @brief Given a string with a label in it, create a new token containing the label and return it.
 * 
 * @param string the string with a label at the start of it
 * @return struct program_token* a pointer to a new calloc'd program_token configured as a label token
 */
struct program_token* tokenizer_makeLabelToken(char* string);


/**
 * @brief Given a string with an opcode in it, create a new token containing the opcode and return it.
 * 
 * @param string the string with an opcode at the start of it
 * @return struct program_token* a pointer to a new calloc'd program_token configured as an opcode token
 */
struct program_token* tokenizer_makeOpcodeToken(char* string);


/**
 * @brief Checks to see if the first word in the string passed is a label or not. Also performs sanity check on label
 *        naming scheme.
 * 
 * @param c the string to check the first word of
 * @return uint8_t 1 if there is a label, else 0.
 */
uint8_t tokenizer_hasLabel(char* c);

/**
 * @brief Checks to see if the first word in the string is a preprocessor directive
 * 
 * @param c the string to check the first word of
 * @return uint8_t 1 if there is a preprocessor directive, else 0
 */
uint8_t tokenizer_hasPreprocessorDirective(char* c);

/**
 * @brief Checks to see if the first word in the string passed matches the predefined opcode list
 * 
 * @param c the string to check the first word of
 * @return uint8_t 1 if there is an opcode, else 0
 */
uint8_t tokenizer_hasOpcode(char* c);


#endif