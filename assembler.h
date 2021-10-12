#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "fileHandler.h"
#include "datastructures.h"
#include "tokenizer.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////                                       VARIABLES AND CONSTANTS                                          ////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Tracks the address the next instruction in the program will go in in the ROM hex file.
//This is a discrete variable because of the plan to allow for preprocessor statements to specify where in memory
//an instruction goes.
uint16_t nextInstructionAddressInROM = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////                                               FUNCTIONS                                                ////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief Is given the paths to an input file, and an output file, and assembles the input file using the Unibit
 *        assembly language, placing binary output in the output file.
 * 
 * @param inputFile  Path to the input file of code to be assembled.
 * @param outputFile Path to output the binary ROM data to.
 */
void assembler_assemble(char* inputFile, char* outputFile);

/**
 * @brief takes in the raw lines of text from a file and makes 
 * 
 * @param text char** to the raw text of the file. The first dimension is the line, and the second, chars within that
 *             line.
 */
struct instruction_token* assembler_tokenizeText(char** text, uint16_t numLines);


void assembler_haltAssembly();


/**
 * @brief completes the assembly process, taking the token's data and setting the raw binary data contained within it.
 * 
 * @param token the instruction to assemble
 */
void assembler_assembleToken(struct instuction_token token);

#endif