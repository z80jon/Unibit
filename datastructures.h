#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include <stdint.h>

enum programTokenType{PROGTOK__INSTRUCTION, PROGTOK__LABEL, PROGTOK__PREPROC_DIR, PROGTOK__VARIABLE_DEC};

enum libraryTokenType{LIBTOK_UNDEFINED, LIBTOK__LABEL, LIBTOK__VARIABLE};

struct program_token {
    char* instruction_text;             ///the raw text of the code--minus spaces and /r/n--between one ; and the next ;.
    enum programTokenType tokenType;         ///The opcode the CPU will execute
    uint16_t address;                   ///The address to jump to or load/store from. Optional in negate instructions.
    struct program_token* nextToken;    ///pointer to the next token in the chain. NULL = end of code
    struct program_token* prevToken;    ///pointer to the previous token in the chain. NULL = start of code.
    uint16_t lineNumber;
};

//Library token system: uses ArrayList


struct library_token {
    char* name;
    enum libraryTokenType libraryTokenType;
    uint16_t address;
    uint16_t size_if_variable;
    uint8_t address_defined;
};

#endif