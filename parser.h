#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "library.h"

#define DEBUG_PARSER 0

enum parser_state{PARSERSTATE__INITIAL_NUMBER, PARSERSTATE__ADD, PARSERSTATE__SUBTRACT, PARSERSTATE__MULTIPLY, PARSERSTATE__INDEX, PARSERSTATE__EXPECTING_OPERATOR};


/**
 * @brief Parses the mathematical value of the text passed to it. Currently only supports converting a hex or dec number
 *        to its literal value. Will be expanded with time.
 * 
 * @param text the text to parse
 * @param returnValue the pointer to place the return value in
 * 
 * @return 0 if parsed successfully, else 1
 */
uint8_t parser(char* text, uint16_t* returnValue);


/**
 * @brief Obtains the discrete value of a non-operator token, deciphering it to be a label, address, hex, or dec number,
 *        and returns this value in returnValue
 * 
 * @param text the token text to be parsed
 * @param returnValue pointer to where to store the decoded value of the token text
 * @return uint8_t 1 if unable to decode token, else 0.
 */
uint8_t parser__get_value_of_token(char* text, uint16_t* returnValue);


/**
 * @brief The parser handles small bits of text, called (poor choice, given it's used elsewhere) tokens. These are just
 *        each char* of a math problem. For instance, "1+12" would be broken up into "1" "+" "12", which are then
 *        interpreted by the parser.
 * 
 *        This function is a glorified macro that just identifies end-of-token symbols.
 * 
 * @param c the char to check for the presence of an end-of-token symbol
 * @return uint8_t 1 if end of token, else 0
 */
uint8_t parser_internal__is_end_of_token(char c);


#endif