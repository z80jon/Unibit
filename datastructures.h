#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include <stdint.h>

enum programTokenType{PROGTOK__HEAD, PROGTOK__INSTRUCTION, PROGTOK__LABEL, PROGTOK__PREPROC_DIR, PROGTOK__VARIABLE_DEC};//TODO change to uppercase first letter

enum Opcode{UNDEFINED, LOAD, STORE, NEGATE, JUMPIFZERO};

enum libraryTokenType{LIBTOK_UNDEFINED, LIBTOK__LABEL, LIBTOK__VARIABLE};//TODO change to uppercase first letter

struct program_token {
    char* instruction_text;             ///the raw text of the code--minus spaces and /r/n--between one ; and the next ;.
    enum Opcode opcode;                 ///If this is a PROGTOK__INSTRUCTION token, this will be populated with the opcode enum
    enum programTokenType tokenType;    ///The type of token this is
    uint16_t romData;                   ///Post-assembly, this will contain the raw data to be stored in EEPROMs.
    struct program_token* nextToken;    ///pointer to the next token in the chain. NULL = end of code
    struct program_token* prevToken;    ///pointer to the previous token in the chain. NULL = start of code.
    uint16_t lineNumber;                ///The line (in the input file) that corresponds to this 
    uint16_t romAddress;                ///The hexadecimal address the instruction will be stored in in ROM
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