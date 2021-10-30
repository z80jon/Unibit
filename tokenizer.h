#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "datastructures.h"
#include "fileHandler.h"
#include <string.h>




static const char* OPCODE_STRINGS[] = {"l ", "load", "s", "store", "negate", "n", "jumpifzero", "jiz", "jump"};
#define OPCODE_STRINGS_LENGTH 9

static const char* LOAD_STRINGS[] = {"l", "load"};
#define LOAD_STRINGS_LENGTH  2
static const char* STORE_STRINGS[] = {"s", "store"};
#define STORE_STRINGS_LENGTH  2
static const char* NEGATE_STRINGS[] = {"n", "negate"};
#define NEGATE_STRINGS_LENGTH  2
static const char* JUMP_STRINGS[] = {"jumpifzero", "jump", "jiz"};
#define JUMP_STRINGS_LENGTH  3




//====================== Main Method ======================//

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




//====================== Helper Functions ======================//

/**
 * @brief Given a string with a preprocessor directive in it, create a new token containing the directive and return it.
 * 
 * @param string the string with the preprocessor directive
 * @return struct program_token* a pointer to a new calloc'd program_token configured as a preprocessor token
 */
struct program_token* tokenizer__make_preprocessor_token(char* string);


/**
 * @brief Given a string with a label in it, create a new token containing the label and return it.
 * 
 * @param string the string with a label at the start of it
 * @return struct program_token* a pointer to a new calloc'd program_token configured as a label token
 */
struct program_token* tokenizer__make_label_token(char* string);


/**
 * @brief Given a string with an opcode in it, create a new token containing the opcode and return it.
 * 
 * @param string the string with an opcode at the start of it
 * @return struct program_token* a pointer to a new calloc'd program_token configured as an opcode token
 */
struct program_token* tokenizer__make_opcode_token(char* string);


/**
 * @brief Given a string with a variable declaration inb it, create a new token containing the variable info and return it.
 * 
 * @param string the string with the variable declaration at the start of it
 * @return struct program_token* a pointer to a new calloc'd program_token configured as a variable declaration token.
 */
struct program_token* tokenizer__make_variable_declaration_token(char* string);


/**
 * @brief Creates a generic token containing instruction_text and returns it. Handles malloc'ing and assigning default values automatically.
 * 
 * @param instruction_text the text to store in token->instruction_text
 * @param tokenType the programTokenType enum to store within the token
 * @return struct program_token* the newly created token
 */
struct program_token* tokenizer__make_generic_token(char* instruction_text, enum programTokenType tokenType);


/**
 * @brief Removes the token passed to it from the chain, deallocating memory as able, and updating the pointers for the neighbor tokens.
 * 
 * @param token the token to remove from the chain.
 * @return struct program_token* the next token in the chain, if there is one.
 */
struct program_token* tokenizer__remove_token_from_chain(struct program_token* token);




//====================== String-Utility Functions ======================//

/**
 * @brief Checks to see if the first word in the string passed is a label or not. Also performs sanity check on label
 *        naming scheme.
 * 
 * @param c the string to check the first word of
 * @return uint8_t 1 if there is a label, else 0.
 */
uint8_t tokenizer__has_label(char* c);


/**
 * @brief Checks to see if the first word in the string is a preprocessor directive
 * 
 * @param c the string to check the first word of
 * @return uint8_t 1 if there is a preprocessor directive, else 0
 */
uint8_t tokenizer__has_preprocessor_directive(char* c);


/**
 * 
 * @brief Checks to see if the first word in the string passed matches the predefined opcode list
 * 
 * @param c the string to check the first word of
 * @return uint8_t 1 if there is an opcode, else 0
 */
uint8_t tokenizer__has_opcode(char* c);


/**
 * @brief Checks to see if the first word in the string is the beginning of a variable declaration
 * 
 * @param c the string to check the first word of
 * @return uint8_t 1 if there is a variable declaration, else 0
 */
uint8_t tokenizer__has_variable(char* c);


/**
 * @brief Prints out metadata about the token, if any exists
 * 
 * @param t the token to print out data from
 */
void tokenizer__print_out_token(struct program_token* t);




#endif