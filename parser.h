#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "library.h"

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

uint8_t parser_getValueOfToken(char* text, uint16_t* returnValue);

uint8_t parser_internal__isEndOfToken(char c);

#endif