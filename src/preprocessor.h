#ifndef PREPROCESSOR_H_
#define PREPROCESSOR_H_

#include <stdint.h>
#include "datastructures.h"


/**
 * @brief Runs the preprocessor, performing the following actions:
 * -logging variables and labels in the symbol library
 * 
 * 
 * @param head 
 * @return uint8_t 0 if successful, else 1
 */
uint8_t preprocessor_run(struct programToken_t* head);




#endif