#ifndef TOKEN_H_
#define TOKEN_H_


#include <stdint.h>


enum tokenType{constant, address, variable};

struct Token{
    char name[16];      //
    uint8_t nameLength; //Test
    tokenType type;     //Tells the assembler what 

};

/**
 * @brief Determines if the string passed is currently in the token
 * 
 * @param stringToCheckForToken The string (don't care if spaces on edges or if not capitalized) to check the list of
 *                              tokens to see if there's a hit
 * 
 * @param lengthOfString the number of bytes in the string
 * @return uint8_t 1 if in token database
 *                 0 if not in token database
 */
uint8_t token_isToken(char* stringToCheckForToken, uint16_t lengthOfString);


//1: read in lines
//2: split lines into unidentified tokens

//End: output machine code

//L immediate_address
//N
//S immediate_address
//J immediate_address
//(blank spaces)

#endif