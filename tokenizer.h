#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "datastructures.h"

//====================== Main Method ======================//


/**
 * \brief Given an input file, splits the text up into discrete tokens that can then be
 *        easily parsed by the preprocessor.
 * 
 * \param inputFile path to the input text file
 * \return struct programToken_t* the head token of the linked list
 */
struct programToken_t* tokenizer_tokenize(char* inputFile);


//====================== Helper Functions ======================//

/**
 * @brief Removes the token passed to it from the chain, updating the next and
 *        previous token's references if they exist, and finally returns the token
 *        following it.
 * 
 * @param token the token to remove from the chain.
 * @return struct programToken_t* the next token in the chain, if there is one.
 */
struct programToken_t* tokenizer_removeToken(struct programToken_t* token);

//TODO
/**
 * \brief 
 * 
 * \param t 
 */
void tokenizer_printOutToken(struct programToken_t* t);


#endif