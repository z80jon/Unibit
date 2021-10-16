#ifndef LIBRARY_H_
#define LIBRARY_H_

#include <stdint.h>
#include "datastructures.h"

//Library Error return values
#define LIBRARY_ERROR__NO_ERRORS        0 ///Function call exited normally
#define LIBRARY_ERROR__NAME_NOT_FOUND   1 ///The name of the requested variable was not found
#define LIBRARY_ERROR__NAME_EXISTS      2 ///The name of the variable or other entity being added to the library is already in use
#define LIBRARY_ERROR__SYNTAX           3 ///The syntax ot the variable or other enttiy being added to the library is not valid. (eg, '%' is in the name of a variable)
#define LIBRARY_ERROR__ADDR_CONFLICT    4 ///The variable with a specified address encroaches on an existing variable's address space.
#define LIBRARY_ERROR__ADDR_UNASSIGNED  5 ///The address for the specified variable has not yet been assigned. TODO define var addr space assign func
#define LIBRARY_ERROR__MEMORY_TRAFFIC   6 ///The variable address assigning routine has failed to allocate address space to all variables.


static struct library_token* libraryTokens; ///Pointer to array of Library tokens
static uint16_t numLibraryTokens;           ///Holds the current number of library tokens


//====================== Variable functions ======================//

/**
 * @brief Adds a record for a variable to the registry in the library.
 * 
 * @param name the name of the variable
 * @param numBits the number of bits to allocate to the variable
 * 
 * @return uint8_t LIBRARY_ERROR__NO_ERRORS, LIBRARY_ERROR__NAME_EXISTS, LIBRARY_ERROR__SYNTAX
 */
uint8_t library_addVariable(char* name, uint16_t numBits);


/**
 * @brief Adds a record for a variable to the registry in the library, and also explicitly predefines its address in memory.
 * 
 * @param name the name of the variable
 * @param numBits the number of bits to allocate to the variable
 * @param address the starting address for the variable (address increases with numBits)
 * @return uint8_t LIBRARY_ERROR__NO_ERRORS, LIBRARY_ERROR__NAME_EXISTS, LIBRARY_ERROR__SYNTAX, LIBRARY_ERROR__ADDR_CONFLICT
 */
uint8_t library_addVariableWithAddress(char* name, uint16_t numBits, uint16_t address);


/**
 * @brief Retrieves the address (in RAM) of the specified variable
 * 
 * @param name the name of the variable
 * @param address a pointer which will be populated with the address
 * @return uint8_t LIBRARY_ERROR__NO_ERRORS, LIBRARY_ERROR__NAME_NOT_FOUND, LIBRARY_ERROR__ADDR_UNASSIGNED
 */
uint8_t library_getVariableAddress(char* name, uint16_t* address);


/**
 * @brief Goes through the variables with no address assigned to them and assigns address space.
 * 
 * @return uint8_t LIBRARY_ERROR__NO_ERRORS, LIBRARY_ERROR__MEMORY_TRAFFIC
 */
uint8_t library__setVariableAddresses();


//====================== Label functions ======================//

/**
 * @brief Adds a label to the dictionary, but does not define its address yet.
 * 
 * @param name the name of the label
 * @return uint8_t LIBRARY_ERROR__NO_ERRORS, LIBRARY_ERROR__NAME_EXISTS, LIBRARY_ERROR__SYNTAX
 */
uint8_t library_addLabel(char* name);


/**
 * @brief Adds a label to the dictionary with an explicitly defined address that the label resolves to.
 * 
 * @param name the name of the label
 * @param rom_address the exact ROM address that the label points to
 * @return uint8_t LIBRARY_ERROR__NO_ERRORS, LIBRARY_ERROR__NAME_EXISTS, LIBRARY_ERROR__SYNTAX
 */
uint8_t library_addLabelWithAddress(char* name, uint16_t rom_address);


/**
 * @brief Sets the address of the label specified.
 * 
 * @param name the name of the label to set the address of
 * @param rom_address the address to set the label to point to
 * @return uint8_t LIBRARY_ERROR__NO_ERRORS, LIBRARY_ERROR__NAME_NOT_FOUND, 
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


//====================== Misc functions ======================//

/**
 * @brief called at the end of assembly--frees up any allocated memory.
 * 
 */
void library__free_memory();


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
 * @return uint8_t LIBRARY_ERROR__NO_ERRORS, LIBRARY_ERROR__SYNTAX
 */
uint8_t library_internal__check_syntax_of_name(char* name);



#endif