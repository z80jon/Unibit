#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_

#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>


/**
 * @brief Reads in the file in path fileName (relative to folder program is executed in)
 * 
 * @param fileName the name of the file to read in
 * @param linesOfTextFromFile a malloc'd pointer to the lines of text.
 * @return int 0 if successful
 */
int fileHandler_readInFile(char* fileName, char*** linesOfTextFromFile, uint32_t* numberOfLines);


#endif