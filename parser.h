#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "library.h"

#define DEBUG_PARSER 1


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
 * \brief Evaluates if the equation in the text should be a logical true or false condition, but with
 *        no variables permitted--any reference to a variable will throw an error.
 * 
 * \param text the text to evaluate for true or false conditional status
 * \return true returned if the text parses to any value other than zero
 * \return false returned if the text parses to be equal to zero
 */
bool parser__evaluateTrueOrFalse_noVariables(char* text);



#endif