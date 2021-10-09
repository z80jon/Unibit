#ifndef INSTRUCTION_TOKEN_H_
#define INSTRUCTION_TOKEN_H_

#include <stdint.h>

/**
 * @brief Defines what stage an instruction_token is at in the assembly process
 * 
 * INIT: the text from the instruction has been put into char* instruction_text
 * DONE: The instruction has finished being assembled, and binaryOutput has valid binary data.
 * 
 */
enum tokenStage{INIT,
                DONE};

struct instruction_token {
    char* instruction_text; //the raw text of the code--minus spaces and /r/n--between one ; and the next ;.
    uint16_t binaryOutput;  //The output machine code post-assembly
    struct instruction_token* nextToken; //pointer to the next token in the chain. NULL = end of code
};

#endif