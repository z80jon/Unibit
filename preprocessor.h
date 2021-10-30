#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_

#include "tokenizer.h"
#include "library.h"
#include "datastructures.h"
#include "parser.h"
#include <stdio.h>




/**
 * @brief Runs the preprocessor, performing the following actions:
 * -logging variables and labels in the symbol library
 * 
 * 
 * @param head 
 * @return uint8_t 0 if successful, else 1
 */
uint8_t preprocessor__run(struct program_token* head);




#endif