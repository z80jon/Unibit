#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include <stdint.h>

enum tokenType{INSTRUCTION, LABEL, PREPROCESSOR_DIRECTIVE, VARIABLE_DECLARATION};

enum opCode{LOAD, STORE, NEGATE, JUMPIFZERO};

struct program_token {
    char* instruction_text; //the raw text of the code--minus spaces and /r/n--between one ; and the next ;.
    enum tokenType typeOfToken;     //The opcode the CPU will execute
    uint16_t address;       //The address to jump to or load/store from. Optional in negate instructions.
    struct program_token* nextToken; //pointer to the next token in the chain. NULL = end of code
    struct program_token* prevToken; //pointer to the previous token in the chain. NULL = start of code.
    uint16_t lineNumber;
};

#endif