#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "datastructures.h"

#define FILEHANDLER_DEBUG 0


/**
 * @brief Reads in the file in path filePath (relative to folder program is executed in)
 * 
 * @param fileName the name of the file to read in
 * @param linesOfTextFromFile a malloc'd pointer to the lines of text.
 * @return int 0 if successful
 */
int fileHandler__read_in_file(char* filePath, char*** linesOfTextFromFile, uint32_t* numberOfLines);


/**
 * @brief Checks if the file specified exists
 * 
 * @param filePath the path to the file in question (relative to the folder the assembler is running in)
 * @return uint8_t 1 if the file exists, 0 if it does not.
 */
uint8_t fileHandler__does_file_exist(char* filePath);


/**
 * @brief takes a line of text, and returns only the useful parts of it, stripping out comments, blank lines,
 *        spaces between words (leaving just 1), etc.
 * 
 */
char* fileHandler__sterilize_text(char* text);


/**
 * @brief Generates the Intel-formatted hex files from the program tokens.
 * 
 * @param head the start of the program
 * @param highByte the file name to output high byte Intel hex data into
 * @param lowByte the file name to output low byte Intel hex data into
 * @return uint8_t 0 if successful, else 1 (likely I/O issue)
 */
uint8_t fileHandler__output_hex_data(struct program_token* head, char* highByteFileName, char* lowByteFileName);


#endif