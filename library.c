#include "library.h"
#include "datastructures.h"


//====================== Library Data Structures ======================//

enum eLibTokenType {
    eLIBTOKEN_UNKNOWN = 0,  //< Unknown token type. Assigned to generic tokens by default.
    eLIBTOKEN_VARIABLE,     //< Indicates that void* token is a libraryToken_variable
    eLIBTOKEN_LABEL         //< Indicates that void* token is a libraryToken_label
};

uint16_t numLibraryTokens = 0;           ///Holds the current number of library tokens

struct libraryToken_t** libraryTokens = NULL; ///Pointer to array of Library tokens

enum eLibTokenType library_getTokenType(char* name);

/**
 * \brief This data structure is the heart of the library. It is extended using the void*
 *        to perform any variety of tasks
 */
struct libraryToken_t {
    char* name;                 //< The token's name
    enum eLibTokenType type;    //< The type of token this is
    void* data;                 //< Pointer to relevant datastructure
};


struct libraryToken_variable_t {
    uint16_t numBits;   //< The number of bits the variable takes up
    uint32_t address;   //< The address (hex) of the variable
    bool bAddrResolved; //< True if the address has been set. Else false.
};


struct libraryToken_label_t {
    struct programToken_t* instr; //< The instruction whose address the label points to
};

//====================== Variable Functions ======================//

// uint8_t library_addVariable(char* name, uint16_t numBits) {
//     if(numBits == 0) return LIBRARY_STATUS__INVALID_SIZE;
//     uint8_t errorCode = addGenericToken(name);
//     if(errorCode != LIBRARY_STATUS__NO_ERRORS)
//         return errorCode;

//     struct libraryToken_t* token = getToken(name);
//     token->size_if_variable = numBits;
//     token->eLibTokenType = LIBTOK__VARIABLE;

//     return LIBRARY_STATUS__NO_ERRORS;
// }


// uint8_t library_addVariableWithAddress(char* name, uint16_t numBits, uint16_t address) {
//     if(numBits == 0) return LIBRARY_STATUS__INVALID_SIZE;
//     if(checkAddressConflict(numBits, address) != LIBRARY_STATUS__NO_ERRORS)
//         return LIBRARY_STATUS__ADDR_CONFLICT;
    
//     uint8_t errorCode = addGenericToken(name);
//     if(errorCode != LIBRARY_STATUS__NO_ERRORS)
//         return errorCode;

//     struct libraryToken_t* token = getToken(name);
//     token->size_if_variable = numBits;
//     token->eLibTokenType = LIBTOK__VARIABLE;
//     token->address = address;
//     token->address_defined = 1;

//     return LIBRARY_STATUS__NO_ERRORS;
// }


uint8_t library_getVariableAddress(char* name, uint16_t* address) {
    uint8_t status = LIBRARY_STATUS__NO_ERRORS;
    struct libraryToken_t* token = getToken(name);

    if(token == NULL)
        status = LIBRARY_STATUS__NAME_NOT_FOUND;
    else if(token->type != eLIBTOKEN_VARIABLE)
        status = LIBRARY_STATUS__TOKEN_NOT_A_VAR;
    else {
        struct libraryToken_variable_t* tokenData;
        tokenData = (struct libraryToken_variable_t*) token->data;

        if(tokenData->bAddrResolved == true) {
            *address = tokenData->address;
        } else {
            status = LIBRARY_STATUS__ADDR_UNASSIGNED;
        }
    }

    return status;    
}


uint8_t library_assignVariableAddresses() {
    struct libraryToken_variable_t* variableData;
    uint8_t status = LIBRARY_STATUS__NO_ERRORS;

    if(DEBUG_LIBRARY)printf("\n[Library]: Assigning variable addresses...");
    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        if(libraryTokens[i]->type == eLIBTOKEN_VARIABLE)
        {
            variableData = (struct libraryToken_variable_t*)libraryTokens[i]->data;
            
            //If there isn't an assigned address, give it the first open slot found
            if(variableData->bAddrResolved == false) {
                for(uint16_t addr = 0; (addr + variableData->numBits) < 0xFFFF; addr++) {
                    if(checkAddressConflict(variableData->numBits, addr) == LIBRARY_STATUS__NO_ERRORS) {
                        variableData->bAddrResolved = true;
                        variableData->address = addr;
                        break;
                    }
                }

                //If after searching, we could not find a valid spot to put the address, throw an error
                if(variableData->bAddrResolved == false) {
                    printf("\n[Library]: [Error]: Variable address space assignment failed! Reason: unable to find space for variable \"%s\"! This could be due to several factors:",libraryTokens[i]->name);
                    printf("\n\t1) If addresses are manually specified, there is overlap with another variable\n\t2) All memory is in use\n\t3) Manual address specification has fragmented the memory such that a location cannot be found");
                    printf("\n\t4) The primitive algorithm in use by the address space assigner could not allocate blocks adequately so as to find a spot. (In this case, rearranging declarations to put large variables first may help)");

                    status = LIBRARY_STATUS__MEMORY_TRAFFIC;
                    break;
                }
            }

        }
    }

    if(DEBUG_LIBRARY)printf("\n[Library]: Variable address space assignment complete!");
    return status;
}


uint8_t library_get_variable_size(char* name, uint16_t* size) {
    uint8_t status = LIBRARY_STATUS__NO_ERRORS;
    struct libraryToken_t* token = getToken(name);
    struct libraryToken_variable_t* variableData;

    if(token == NULL)
        status = LIBRARY_STATUS__NAME_NOT_FOUND;
    else if(token->type != eLIBTOKEN_VARIABLE) {
        status = LIBRARY_STATUS__TOKEN_NOT_A_VAR;
    } else {
        variableData = (struct libraryToken_variable_t*) token->data;
        *size = variableData->numBits;
    }

    return status;
}




//====================== Label Functions ======================//

uint8_t library_addLabel(struct programToken_t* token) {
    char* labelName = ((struct label_t*)token->data)->text;
    uint8_t status = addGenericToken(labelName);
    if(status != LIBRARY_STATUS__NO_ERRORS)
        return status;

    struct libraryToken_t* label = getToken(labelName);

    //Find the next instruction token
    struct programToken_t* t = token->next;
    while(t != NULL && t->type != ePROGTOKEN_INSTRUCTION) {
        t = t->next;
    }

    //Save it
    ((struct libraryToken_label_t*)label->data)->instr = t;

    
    return LIBRARY_STATUS__NO_ERRORS;
}


// uint8_t library_addLabelWithAddress(char* name, uint16_t rom_address) {
//     uint8_t errorCode = addGenericToken(name);
//     if(errorCode != LIBRARY_STATUS__NO_ERRORS)
//         return errorCode;

//     struct libraryToken_t* token = getToken(name);
//     token->eLibTokenType = LIBTOK__LABEL;
//     token->address_defined = 1;
//     token->address = rom_address;

//     return LIBRARY_STATUS__NO_ERRORS;
// }


// uint8_t library_setLabelAddress(char* name, uint16_t rom_address) {
//     struct libraryToken_t* token = getToken(name);
//     if(token == NULL)
//         return LIBRARY_STATUS__NAME_NOT_FOUND;
    
//     token->address_defined = 1;
//     token->address = rom_address;

//     return LIBRARY_STATUS__NO_ERRORS;
// }


// uint8_t library_getLabelAddress(char* name, uint16_t* rom_address) {
//     struct libraryToken_t* token = getToken(name);
//     if(token == NULL)
//         return LIBRARY_STATUS__NAME_NOT_FOUND;
//     if(token->pointer_if_label == NULL)
//         return LIBRARY_STATUS__ADDR_UNASSIGNED;
    
//     *rom_address = token->address;
//     return LIBRARY_STATUS__NO_ERRORS;
// }


// uint8_t library_ResolveLabelAddresses(struct programToken_t* head) {
//     struct programToken_t* labelPointer; //Advances along the linked list seeking out labels to point to opcode ROM addresses
//     struct programToken_t* opcodePointer;//Advances along the linked list just ahead of labelPointer to find the opcode the label should point to.
//     if(DEBUG_LIBRARY)printf("\n[Library]: Resolving label addresses...");
//     labelPointer = head;
//     opcodePointer = head;
//     while(labelPointer != NULL) {
//         //printf("\nOn label \"%s\" and search is on \"%s\"",labelPointer->instruction_text,opcodePointer->instruction_text);
//         if(labelPointer->type == eLIBTOKEN_LABEL) {
//             opcodePointer = labelPointer->nextToken;
//             while(opcodePointer->type != eTOKEN_INSTRUCTION && opcodePointer != NULL) {
//                 opcodePointer = opcodePointer->nextToken;
//             }

//             //Bad user input case: a label was declared and that's just the end of the program--no opcodes follow
//             if(opcodePointer == NULL) {
//                 printf("\n[Library]: [Error]: Label address resolution failed! Dangling label with name \"%s\" detected!",labelPointer->instruction_text);
//                 return LIBRARY_STATUS__DANGLING_LABEL;
//             }

//             if(library_setLabelAddress(labelPointer->instruction_text, opcodePointer->romAddress) != LIBRARY_STATUS__NO_ERRORS) {
//                 printf("\n[Library]: [FATAL ERROR]: Tried to set address of label '%s', but it was not in the dictionary! This should only ever be printed if something has gone wrong with the library code.",labelPointer->instruction_text);
//                 return LIBRARY_STATUS__UNKNOWN_ERROR;
//             } else {
//                 //printf("\n[Library]: Set label \"%s\" to point to instruction with text \"%s\"",labelPointer->instruction_text,opcodePointer->instruction_text);
//             }

//         }
        
//         labelPointer = labelPointer->nextToken;
//     }

//     if(DEBUG_LIBRARY)printf("\n[Library]: Label address resolution complete!");
//     return LIBRARY_STATUS__NO_ERRORS;
// }




//====================== Misc Functions ======================//

void library_freeMemory() {
    printf("\n======== Library Tokens =======\n");

    //(For debug purposes: print out the token info):
    //First: label tokens:
    // printf("\n== Label Tokens ==\n");
    // for(uint16_t i = 0; i < numLibraryTokens; i++) {
    //     if(libraryTokens[i].type == LIBTOK__LABEL) {
    //         printf("Label name: %s, address: %d/0x%X\n",libraryTokens[i].name, libraryTokens[i].address,libraryTokens[i].address);
    //     }
    // }


    // //Next: variable tokens:
    // printf("\n== Variable Tokens ==\n");
    // for(uint16_t i = 0; i < numLibraryTokens; i++) {
    //     if(libraryTokens[i].eLibTokenType == LIBTOK__VARIABLE) {
    //         printf("Variable name: %s, number of bits: %d, address: %d/0x%X\n",libraryTokens[i].name, libraryTokens[i].size_if_variable,libraryTokens[i].address,libraryTokens[i].address);
    //     }
    // }

    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        free(libraryTokens[i]->name);
    }

    free(libraryTokens);
}


enum eLibTokenType library_getTokenType(char* name) {
    struct libraryToken_t* token = getToken(name);
    if(token == NULL)
        return LIBRARY_STATUS__NAME_NOT_FOUND;
    
    return token->type;
}




//====================== Internal-use Functions ======================//

struct libraryToken_t* getToken(char* name) {
    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        if(strcmp(libraryTokens[i]->name, name) == 0)
            return libraryTokens[i];
    }

    return NULL;
}


uint8_t checkSyntaxOfName(char* name) {

    //If the string is >255 chars long, or starts with a number, that is a syntax error.
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


uint8_t addGenericToken(char* name) {
    
    //First: verify that the name of the token will pass syntax checks, and is not already in use.
    uint8_t errorCode = checkSyntaxOfName(name);
    if(errorCode != LIBRARY_STATUS__NO_ERRORS)
        return errorCode;
    if(getToken(name) != NULL)
        return LIBRARY_STATUS__NAME_EXISTS;

    numLibraryTokens++;
    libraryTokens = (struct libraryToken_t**) realloc(libraryTokens, sizeof(struct libraryToken_t)*numLibraryTokens);

    libraryTokens[numLibraryTokens-1]->name = malloc(sizeof(char) * strlen(name)+1);
    strcpy(libraryTokens[numLibraryTokens-1]->name, name);

    libraryTokens[numLibraryTokens-1]->type = eLIBTOKEN_UNKNOWN;

    return LIBRARY_STATUS__NO_ERRORS;
}


uint8_t checkAddressConflict(uint16_t numBits, uint16_t address) {
    for(uint16_t i = 0; i < numLibraryTokens; i++) {
        if(libraryTokens[i]->type == eLIBTOKEN_VARIABLE && 
        ((struct libraryToken_variable_t*) libraryTokens[i]->data)->bAddrResolved == true) {
            uint16_t tokenAddr = ((struct libraryToken_variable_t*) libraryTokens[i]->data)->address;
            uint16_t tokenSize = ((struct libraryToken_variable_t*) libraryTokens[i]->data)->numBits;

            if((address == tokenAddr) ||                                        //Same starting address
               ((address < tokenAddr) && ((address + numBits) > tokenAddr)) ||  //Start address below libraryTokens[i] but extends into it
               ((address > tokenAddr) && ((tokenAddr + tokenSize) > address)))  //Start address above libraryTokens[i] but libraryTokens[i] extends into it
                return LIBRARY_STATUS__ADDR_CONFLICT;
        }
    }

    return LIBRARY_STATUS__NO_ERRORS;
}

