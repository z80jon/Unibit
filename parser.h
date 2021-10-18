#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "library.h"

/**
 * @brief Parses the mathematical value of the text passed to it. Currently only supports converting a hex or dec number
 *        to its literal value. Will be expanded with time.
 * 
 * @param text the text to parse
 * @return uint16_t the value the text comes out to
 */
uint16_t parser(char* text);


#endif