#ifndef fileIO_H_
#define fileIO_H_

#include "datastructures.h"




#define fileIO_DEBUG 0



/**
 * @brief Reads in the file in path filePath (relative to folder program is executed in)
 * 
 * @param fileName the name of the file to read in
 * @param linesOfTextFromFile a malloc'd pointer to the lines of text.
 * @return int 0 if successful
 */
int fileIO_readInFile(char* filePath, char*** linesOfTextFromFile, uint32_t* numberOfLines);


/**
 * @brief Checks if the file specified exists
 * 
 * @param filePath the path to the file in question (relative to the folder the assembler is running in)
 * @return uint8_t 1 if the file exists, 0 if it does not.
 */
uint8_t fileIO_doesFileExist(char* filePath);


/**
 * @brief takes a line of text, and returns only the useful parts of it, stripping out comments, blank lines,
 *        spaces between words (leaving just 1), etc.
 * 
 */
char* fileIO_sterilizeText(char* text);


/**
 * @brief Generates the Intel-formatted hex files from the program tokens.
 * 
 * @param head the start of the program
 * @param highByte the file name to output high byte Intel hex data into
 * @param lowByte the file name to output low byte Intel hex data into
 * @return uint8_t 0 if successful, else 1 (likely I/O issue)
 */
uint8_t fileIO_generateIntelHexFile(struct programToken_t* head, char* highByteFileName, char* lowByteFileName);


/**
 * \brief Checks if the file referred to by filePath has already been included in the program in
 *        some capacity. Used to enforce c-like "Include once" capability
 * 
 * \param filePath 
 * \return true 
 * \return false 
 */
bool fileIO_hasFileBeenImportedAlready(char* filePath);



/**
 * \brief Compares str1 and str2 to see if they are identical--but ignoring the case of the
 *        letters therein.
 * 
 * \param str1 the first string to be compared
 * \param str2 the second string to be compared
 * \return true if both strings are identical without respect to case
 * \return false if strings are not identical without respect to case
 */
bool fileIO_equalsIgnoreCase(char const *str1, char const *str2);


/**
 * \brief Checks to see if str1 contains str2 at the very start of it, without regards to
 *        case.
 * 
 * \param str1 the larger string to check for the presence of str2 in
 * \param str2 the target string
 * \return true if str1 contains str2 for the first strlen(str2) characters
 * \return false if str1 < str2 || if the first strlen(str2) characters of str1 are not equal
 *               (without respect to case) to str2.
 */
bool fileIO_containsIgnoreCase(char const *str1, char const *str2);


/**
 * \brief Creates a new string with only the operand from an input already-cleaned line of input
 *        text. Does not modify input string.
 * 
 * \param text cleaned instruction text
 * \return char* pointer to a newly allocated string containg the operand and no leading or
 *               trailing spaces.
 */
char* fileIO_extractAndCleanOperand(char* text);

#endif