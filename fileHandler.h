#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Reads in the file in path filePath (relative to folder program is executed in)
 * 
 * @param fileName the name of the file to read in
 * @param linesOfTextFromFile a malloc'd pointer to the lines of text.
 * @return int 0 if successful
 */
int fileHandler_readInFile(char* filePath, char*** linesOfTextFromFile, uint32_t* numberOfLines);


/**
 * @brief Checks if the file specified exists
 * 
 * @param filePath the path to the file in question (relative to the folder the assembler is running in)
 * @return uint8_t 1 if the file exists, 0 if it does not.
 */
uint8_t fileHandler_doesFileExist(char* filePath);

/**
 * @brief takes a line of text, and returns only the useful parts of it, stripping out comments, blank lines,
 *        spaces between words (leaving just 1), etc.
 * 
 */
char* fileHandler_sterlizeText(char* text);


#endif