#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "fileIO.h"
#include "datastructures.h"
#include "tokenizer.h"
#include "preprocessor.h"




extern struct programToken_t* head; ///Pointer to head node of linked list of program tokens (see datastructures.h for more info)


/**
 * @brief Is given the paths to an input file, and an output file, and assembles the input file using the Unibit
 *        assembly language, placing binary output in the output file.
 * 
 * @param inputFile  Path to the input file of code to be assembled.
 * @param outputFile Path to output the binary ROM data to.
 */
void assembler_run(char* inputFile, char* outputFile);


/**
 * @brief Generates hex ROM data for all of the tokens, referencing the library and parsing data as necessary.
 * 
 * @param head the starting token of the chain
 * @return uint8_t 0 if successful, else 1
 */
uint8_t assembler_generateHex(struct programToken_t* head);




#endif