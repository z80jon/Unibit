#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_

#include "tokenizer.h"
#include "dictionary.h"
#include "datastructures.h"

/**
 * @brief 
 * 
 * @param head 
 * @return uint8_t 0 if successful, else 1
 */
uint8_t preprocessor_run(struct instruction_token head);

#endif