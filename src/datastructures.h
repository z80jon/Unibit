#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include <stdint.h>
#include <stdbool.h>



//====================== Tokenizer / Preprocessor Data Structures ======================//

enum eProgTokenType {
    ePROGTOKEN_INSTRUCTION = 0,
    ePROGTOKEN_LABEL,
    ePROGTOKEN_PREPROCESSOR_DIRECTIVE,
    ePROGTOKEN_VARIABLE
};

extern const char* OPCODE_LIST_NEGATIVE[];
extern const char* OPCODE_LIST_LOAD[];
extern const char* OPCODE_LIST_STORE[];
extern const char* OPCODE_LIST_JUMPIFZERO[];

enum opCode {
    eLOAD = 0,
    eSTORE,
    eJUMPIFZERO,
    eNEGATE,
    eINVALID
};

struct programToken_t {
    enum eProgTokenType type;    //< the type of data structure stored in data
    void* data;                  //< contains token type-specific data
    char* originalText;          //< the original line of code introduced to this token

    struct programToken_t* prev; //< The previous program token in the sequence
    struct programToken_t* next; //< The next program token in the sequence
};

struct instruction_t {
    char* text;                 //< The operand text (pre-parsing)
    enum opCode opcode;         //< The instruction being executed
    uint16_t operand;           //< The operand for the instruction
    uint16_t address;           //< The address (in ROM) that the instruction will reside at
};

struct label_t {
    char* text;
};

//====================== Instruction Data Structures ======================//



#endif