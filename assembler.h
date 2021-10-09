#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "fileHandler.h"
#include "datastructures.h"

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
struct instruction_token assembler_tokenizeText(char** text, uint16_t numLines);


/**
 * @brief takes a line of text, and returns only the useful parts of it, stripping out comments, blank lines,
 *        spaces between words (leaving just 1), etc.
 * 
 */
char* assembler_tokenizeLine(char* text);

#endif