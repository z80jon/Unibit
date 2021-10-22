#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "fileHandler.h"
#include "datastructures.h"
#include "tokenizer.h"
#include "preprocessor.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////                                       VARIABLES AND CONSTANTS                                          ////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Is the 'head' node of the LinkedList of program_tokens that the assembler will step through in the assembly
 *        process.
 */
static struct program_token* head;

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
void assembler_run(char* inputFile, char* outputFile);

/**
 * @brief takes in the raw lines of text from a file and makes 
 * 
 * @param text char** to the raw text of the file. The first dimension is the line, and the second, chars within that
 *             line.
 */
struct instruction_token* assembler_tokenizeText(char** text, uint16_t numLines);


void assembler_haltAssembly();



uint8_t assembler__generate_hex(struct instuction_token token);

#endif