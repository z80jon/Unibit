#ifndef LIBRARY_H_
#define LIBRARY_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "datastructures.h"

//Library Error return values
#define LIBRARY_STATUS__NO_ERRORS        0 ///Function call exited normally
#define LIBRARY_STATUS__NAME_NOT_FOUND   1 ///The name of the requested variable was not found
#define LIBRARY_STATUS__NAME_EXISTS      2 ///The name of the variable or other entity being added to the library is already in use
#define LIBRARY_STATUS__INVALID_SIZE     3 ///A variable with size 0 was attempted to be created.
#define LIBRARY_STATUS__SYNTAX_ERROR     4 ///The syntax ot the variable or other enttiy being added to the library is not valid. (eg, '%' is in the name of a variable)
#define LIBRARY_STATUS__ADDR_CONFLICT    5 ///The variable with a specified address encroaches on an existing variable's address space.
#define LIBRARY_STATUS__ADDR_UNASSIGNED  6 ///The address for the specified variable has not yet been assigned. TODO define var addr space assign func
#define LIBRARY_STATUS__MEMORY_TRAFFIC   7 ///The variable address assigning routine has failed to allocate address space to all variables.
#define LIBRARY_STATUS__UNKNOWN_ERROR    8 ///This error should never occur unless something is wrong in memory allocation or the library code.
#define LIBRARY_STATUS__TOKEN_IS_LABEL   9 
#define LIBRARY_STATUS__TOKEN_IS_VAR     10
#define LIBRARY_STATUS__DANGLING_LABEL   11 ///This error will occur if a label is declared at the end of a program with no instructions following it.

static struct library_token* libraryTokens = NULL; ///Pointer to array of Library tokens
static uint16_t numLibraryTokens = 0;           ///Holds the current number of library tokens

static const uint8_t VALID_NON_ALPHANUM_CHARS_LEN = 1;
static const char VALID_NON_ALPHANUM_CHARS[] = {'_'};

//====================== Variable functions ======================//

/**
 * @brief Adds a record for a variable to the registry in the library.
 * 
 * @param name the name of the variable
 * @param numBits the number of bits to allocate to the variable
 * 
 * @return uint8_t LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__NAME_EXISTS, LIBRARY_STATUS__SYNTAX
 */
uint8_t library_addVariable(char* name, uint16_t numBits);


/**
 * @brief Adds a record for a variable to the registry in the library, and also explicitly predefines its address in memory.
 * 
 * @param name the name of the variable
 * @param numBits the number of bits to allocate to the variable
 * @param address the starting address for the variable (address increases with numBits)
 * @return uint8_t LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__NAME_EXISTS, LIBRARY_STATUS__SYNTAX, LIBRARY_STATUS__ADDR_CONFLICT
 */
uint8_t library_addVariableWithAddress(char* name, uint16_t numBits, uint16_t address);


/**
 * @brief Retrieves the address (in RAM) of the specified variable
 * 
 * @param name the name of the variable
 * @param address a pointer which will be populated with the address
 * @return uint8_t LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__NAME_NOT_FOUND, LIBRARY_STATUS__ADDR_UNASSIGNED
 */
uint8_t library_getVariableAddress(char* name, uint16_t* address);


/**
 * @brief Goes through the variables with no address assigned to them and assigns address space.
 * 
 * @return uint8_t LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__MEMORY_TRAFFIC
 */
uint8_t library_assignVariableAddresses();


//====================== Label functions ======================//

/**
 * @brief Adds a label to the dictionary, but does not define its address yet.
 * 
 * @param name the name of the label
 * @return uint8_t LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__NAME_EXISTS, LIBRARY_STATUS__SYNTAX
 */
uint8_t library_addLabel(char* name);


/**
 * @brief Adds a label to the dictionary with an explicitly defined address that the label resolves to.
 * 
 * @param name the name of the label
 * @param rom_address the exact ROM address that the label points to
 * @return uint8_t LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__NAME_EXISTS, LIBRARY_STATUS__SYNTAX
 */
uint8_t library_addLabelWithAddress(char* name, uint16_t rom_address);


/**
 * @brief Sets the address of the label specified.
 * 
 * @param name the name of the label to set the address of
 * @param rom_address the address to set the label to point to
 * @return uint8_t LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__NAME_NOT_FOUND, 
 */
uint8_t library_setLabelAddress(char* name, uint16_t rom_address);


/**
 * @brief returns the address of the label specified
 * 
 * @param name 
 * @param rom_address 
 * @return uint8_t 
 */
uint8_t library_getLabelAddress(char* name, uint16_t* rom_address);


/**
 * @brief Runs through the program, ensuring that the address each label in the dictionary points to is up to date.
 * 
 * @param head the head node of the program
 * @return LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__UNKNOWN_ERROR LIBRARY_STATUS__DANGLING_LABEL
 */
uint8_t library_resolveLabelAddresses(struct program_token* head);

//====================== Misc functions ======================//

/**
 * @brief called at the end of assembly--frees up any allocated memory.
 * 
 */
void library__free_memory();


enum libraryTokenType library_getTokenType(char* name);

//====================== Internal-use functions ======================//

/**
 * @brief Attempts to locate and return the library token with the specified name
 * 
 * @param name the name of the token to retrieve
 * @return struct library_token the token requested, else 0.
 */
struct library_token* library_internal__getToken(char* name);

/**
 * @brief Checks the syntax of the name against existing naming conventions.
 * 
 * @param name the name to check the syntax of
 * @return uint8_t LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__SYNTAX
 */
uint8_t library_internal__check_syntax_of_name(char* name);

/**
 * @brief Handles reallocating memory and adding a new token to the array. When complete, the new blank token is at the
 *        end of the array. It also checks the syntax of the token, and verifies that the name is not already in use.
 * 
 * @param name the name the token will refer to (be it a label, variable, etc)
 * @returns LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__SYNTAX_ERROR, LIBRARY_STATUS__NAME_EXISTS
 */
uint8_t library_internal__add_generic_token(char* name);

/**
 * @brief Checks for RAM address conflicts with the specified starting address and number of bits following it to see if
 *        any other variables are already occupying some or all of said space.
 * 
 * @param numBits the number of bits following the starting address
 * @param address the address of the first bit in the series of numBits bits.
 * @return LIBRARY_STATUS__NO_ERRORS, LIBRARY_STATUS__ADDR_CONFLICT
 */
uint8_t library_internal__check_for_RAM_address_conflicts(uint16_t numBits, uint16_t address);



#endif