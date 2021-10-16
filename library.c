#include "library.h"


uint8_t library_addVariable(char* name, uint16_t numBits) {
    if(library_internal__getToken(name) != NULL)
        return LIBRARY_ERROR__NAME_EXISTS;
    if(library_internal__check_syntax_of_name(name) == LIBRARY_ERROR__SYNTAX)
        return LIBRARY_ERROR__SYNTAX;


    //TODO
    return LIBRARY_ERROR__NO_ERRORS;
}

uint8_t library_addVariableWithAddress(char* name, uint16_t numBits, uint16_t address) {
    if(library_internal__getToken(name) != NULL)
        return LIBRARY_ERROR__NAME_EXISTS;
    if(library_internal__check_syntax_of_name(name) == LIBRARY_ERROR__SYNTAX)
        return LIBRARY_ERROR__SYNTAX;


    //TODO
    return LIBRARY_ERROR__NO_ERRORS;
}


uint8_t library_getVariableAddress(char* name, uint16_t* address) {
    if(library_internal__getToken(name) == NULL)
        return LIBRARY_ERROR__NAME_NOT_FOUND;
    if(library_internal__check_syntax_of_name(name) == LIBRARY_ERROR__SYNTAX)
        return LIBRARY_ERROR__SYNTAX;

    
    //TODO
}


uint8_t library__setVariableAddresses() {
    //TODO

    return LIBRARY_ERROR__NO_ERRORS;
}




//====================== Label functions ======================//


uint8_t library_addLabel(char* name) {
    if(library_internal__getToken(name) != NULL)
        return LIBRARY_ERROR__NAME_EXISTS;
    if(library_internal__check_syntax_of_name(name) == LIBRARY_ERROR__SYNTAX)
        return LIBRARY_ERROR__SYNTAX;

    //TODO
    return LIBRARY_ERROR__NO_ERRORS;
}


uint8_t library_addLabelWithAddress(char* name, uint16_t rom_address) {
    if(library_internal__getToken(name) != NULL)
        return LIBRARY_ERROR__NAME_EXISTS;
    if(library_internal__check_syntax_of_name(name) == LIBRARY_ERROR__SYNTAX)
        return LIBRARY_ERROR__SYNTAX;

    //TODO
    return LIBRARY_ERROR__NO_ERRORS;
}


uint8_t library_setLabelAddress(char* name, uint16_t rom_address) {
    if(library_internal__getToken(name) == NULL)
        return LIBRARY_ERROR__NAME_NOT_FOUND;

    //TODO
    return LIBRARY_ERROR__NO_ERRORS;
}


uint8_t library_getLabelAddress(char* name, uint16_t* rom_address) {
    if(library_internal__getToken(name) == NULL)
        return LIBRARY_ERROR__NAME_NOT_FOUND;

    //TODO
    return LIBRARY_ERROR__NO_ERRORS;
}


//====================== Misc functions ======================//

void library__free_memory() {
    //TODO
}


//====================== Internal-use functions ======================//


struct library_token* library_internal__getToken(char* name) {
    //TODO
}