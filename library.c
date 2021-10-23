#include "library.h"


uint8_t library_addVariable(char* name, uint16_t numBits) {
    if(numBits == 0) return LIBRARY_STATUS__INVALID_SIZE;
    uint8_t errorCode = library_internal__add_generic_token(name);
    if(errorCode != LIBRARY_STATUS__NO_ERRORS)
        return errorCode;

    struct library_token* token = library_internal__getToken(name);
    token->size_if_variable = numBits;
    token->libraryTokenType = LIBTOK__VARIABLE;

    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_addVariableWithAddress(char* name, uint16_t numBits, uint16_t address) {
    if(numBits == 0) return LIBRARY_STATUS__INVALID_SIZE;
    if(library_internal__check_for_RAM_address_conflicts(numBits, address) != LIBRARY_STATUS__NO_ERRORS)
        return LIBRARY_STATUS__ADDR_CONFLICT;
    
    uint8_t errorCode = library_internal__add_generic_token(name);
    if(errorCode != LIBRARY_STATUS__NO_ERRORS)
        return errorCode;

    struct library_token* token = library_internal__getToken(name);
    token->size_if_variable = numBits;
    token->libraryTokenType = LIBTOK__VARIABLE;
    token->address = address;
    token->address_defined = 1;

    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_getVariableAddress(char* name, uint16_t* address) {
    struct library_token* token = library_internal__getToken(name);
    if(token == NULL)
        return LIBRARY_STATUS__NAME_NOT_FOUND;
    if(token->address_defined == 0)
        return LIBRARY_STATUS__ADDR_UNASSIGNED;
    
    *address = token->address;
    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_assignVariableAddresses() {
    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        struct library_token* token = &(libraryTokens[i]);
        if(token->libraryTokenType == LIBTOK__VARIABLE && token->address_defined == 0) {

            for(uint16_t addr = 0; (addr + token->size_if_variable) < 0xFFFF; addr++) {
                if(library_internal__check_for_RAM_address_conflicts(token->size_if_variable, addr) == LIBRARY_STATUS__NO_ERRORS) {
                    token->address_defined = 1;
                    token->address = addr;
                    break;
                }
            }

            //If we couldn't find free memory for the token, memory is full or too fragmented (by the user, or with this simple placement algorithm)
            //to be able to find locations for every variable.
            if(token->address_defined == 0)
                return LIBRARY_STATUS__MEMORY_TRAFFIC;
        }
    }

    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_getVariableSize(char* name, uint16_t* size) {
    struct library_token* token = library_internal__getToken(name);
    if(token == NULL)
        return LIBRARY_STATUS__NAME_NOT_FOUND;
    if(token->address_defined == 0)
        return LIBRARY_STATUS__ADDR_UNASSIGNED;
    
    *size = token->size_if_variable;
    return LIBRARY_STATUS__NO_ERRORS;    
}


//====================== Label functions ======================//


uint8_t library_addLabel(char* name) {
    uint8_t errorCode = library_internal__add_generic_token(name);
    if(errorCode != LIBRARY_STATUS__NO_ERRORS)
        return errorCode;

    struct library_token* token = library_internal__getToken(name);
    token->libraryTokenType = LIBTOK__LABEL;
    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_addLabelWithAddress(char* name, uint16_t rom_address) {
    uint8_t errorCode = library_internal__add_generic_token(name);
    if(errorCode != LIBRARY_STATUS__NO_ERRORS)
        return errorCode;

    struct library_token* token = library_internal__getToken(name);
    token->libraryTokenType = LIBTOK__LABEL;
    token->address_defined = 1;
    token->address = rom_address;

    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_setLabelAddress(char* name, uint16_t rom_address) {
    struct library_token* token = library_internal__getToken(name);
    if(token == NULL)
        return LIBRARY_STATUS__NAME_NOT_FOUND;
    
    token->address_defined = 1;
    token->address = rom_address;

    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_getLabelAddress(char* name, uint16_t* rom_address) {
    struct library_token* token = library_internal__getToken(name);
    if(token == NULL)
        return LIBRARY_STATUS__NAME_NOT_FOUND;
    if(token->address_defined == 0)
        return LIBRARY_STATUS__ADDR_UNASSIGNED;
    
    *rom_address = token->address;
    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_resolveLabelAddresses(struct program_token* head) {
    struct program_token* labelPointer; //Advances along the linked list seeking out labels to point to opcode ROM addresses
    struct program_token* opcodePointer;//Advances along the linked list just ahead of labelPointer to find the opcode the label should point to.

    labelPointer = head;
    opcodePointer = head;
    while(labelPointer != NULL) {
        //printf("\nOn label \"%s\" and search is on \"%s\"",labelPointer->instruction_text,opcodePointer->instruction_text);
        if(labelPointer->tokenType == PROGTOK__LABEL) {
            opcodePointer = labelPointer->nextToken;
            while(opcodePointer->tokenType != PROGTOK__INSTRUCTION && opcodePointer != NULL) {
                opcodePointer = opcodePointer->nextToken;
            }

            //Bad user input case: a label was declared and that's just the end of the program--no opcodes follow
            if(opcodePointer == NULL) {
                printf("\n[Library]: [FATAL ERROR]: Dangling label with name \"%s\" detected!",labelPointer->instruction_text);
                return LIBRARY_STATUS__DANGLING_LABEL;
            }

            if(library_setLabelAddress(labelPointer->instruction_text, opcodePointer->romAddress) != LIBRARY_STATUS__NO_ERRORS) {
                printf("\n[Library]: [FATAL ERROR]: Tried to set address of label '%s', but it was not in the dictionary! This should only ever be printed if something has gone wrong with the library code.",labelPointer->instruction_text);
                return LIBRARY_STATUS__UNKNOWN_ERROR;
            } else {
                //printf("\n[Library]: Set label \"%s\" to point to instruction with text \"%s\"",labelPointer->instruction_text,opcodePointer->instruction_text);
            }

        }
        
        labelPointer = labelPointer->nextToken;
    }

    return LIBRARY_STATUS__NO_ERRORS;
}

//====================== Misc functions ======================//

void library__free_memory() {
    printf("\n======== Library Tokens =======\n");

    //(For debug purposes: print out the token info):
    //First: label tokens:
    printf("\n== Label Tokens ==\n");
    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        if(libraryTokens[i].libraryTokenType == LIBTOK__LABEL) {
            printf("Label name: %s, address: %d/0x%X\n",libraryTokens[i].name, libraryTokens[i].address,libraryTokens[i].address);
        }
    }


    //Next: variable tokens:
    printf("\n== Variable Tokens ==\n");
    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        if(libraryTokens[i].libraryTokenType == LIBTOK__VARIABLE) {
            printf("Variable name: %s, number of bits: %d, address: %d/0x%X\n",libraryTokens[i].name, libraryTokens[i].size_if_variable,libraryTokens[i].address,libraryTokens[i].address);
        }
    }

    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        free(libraryTokens[i].name);
    }

    free(libraryTokens);
}


enum libraryTokenType library_getTokenType(char* name) {
    struct library_token* token = library_internal__getToken(name);
    if(token == NULL)
        return LIBRARY_STATUS__NAME_NOT_FOUND;
    
    return token->libraryTokenType;
}


//====================== Internal-use functions ======================//


struct library_token* library_internal__getToken(char* name) {
    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        if(strcmp(libraryTokens[i].name, name) == 0)
            return &(libraryTokens[i]);
    }

    return NULL;
}


uint8_t library_internal__check_syntax_of_name(char* name) {

    //If the string is >255 char slong, or starts with a number, that is a syntax error.
    if(strlen(name) > 255 || (isalnum(name[0]) && !isalpha(name[0])) )
        return LIBRARY_STATUS__SYNTAX_ERROR;

    //If the character has a character that isn't alphanumeric, AND isn't in the VALID_NON_ALPHANUM_CHARS, it is OK
    for(uint8_t i = 0; i < strlen(name); i++) {
        if(!isalnum(name[i])) {

            uint8_t invalid = 1;
            for(uint8_t j = 0; j < VALID_NON_ALPHANUM_CHARS_LEN; j++) {
                if(name[i] == VALID_NON_ALPHANUM_CHARS[j])
                    invalid = 0;
            }
            if(invalid)
                return LIBRARY_STATUS__SYNTAX_ERROR;
        }
    }

    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_internal__add_generic_token(char* name) {
    
    //First: verify that the name of the token will pass syntax checks, and is not already in use.
    uint8_t errorCode = library_internal__check_syntax_of_name(name);
    if(errorCode != LIBRARY_STATUS__NO_ERRORS)
        return errorCode;
    if(library_internal__getToken(name) != NULL)
        return LIBRARY_STATUS__NAME_EXISTS;

    
    libraryTokens = (struct library_token*) realloc(libraryTokens, sizeof(struct library_token)*++numLibraryTokens);

    libraryTokens[numLibraryTokens-1].name = malloc(sizeof(char) * strlen(name)+1);
    strcpy(libraryTokens[numLibraryTokens-1].name, name);

    libraryTokens[numLibraryTokens-1].size_if_variable = 0;
    libraryTokens[numLibraryTokens-1].address = 0;
    libraryTokens[numLibraryTokens-1].address_defined = 0;
    libraryTokens[numLibraryTokens-1].libraryTokenType = LIBTOK_UNDEFINED;

    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t library_internal__check_for_RAM_address_conflicts(uint16_t numBits, uint16_t address) {
    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        if(libraryTokens[i].libraryTokenType == LIBTOK__VARIABLE && libraryTokens[i].address_defined == 1) {
            uint16_t tokenAddr = libraryTokens[i].address;
            uint16_t tokenSize = libraryTokens[i].size_if_variable;

            if((address == tokenAddr) ||                                        //Same starting address
               ((address < tokenAddr) && ((address + numBits) > tokenAddr)) ||  //Start address below libraryTokens[i] but extends into it
               ((address > tokenAddr) && ((tokenAddr + tokenSize) > address)))  //Start address above libraryTokens[i] but libraryTokens[i] extends into it
                return LIBRARY_STATUS__ADDR_CONFLICT;
        }
    }

    return LIBRARY_STATUS__NO_ERRORS;
}

