#include "preprocessor.h"
#include <stdbool.h>
#include "tokenizer.h"
#include "library.h"
#include "datastructures.h"
#include "parser.h"
#include <stdio.h>


//====================== Local Functions & Variables ======================//

bool bIncludeToken = true;  //< If false, the token being inspected should be discarded.

enum ePreprocessorDirectiveType {
    PPD_INCLUDE = 0,    //> C-like "#INCLUDE" operator
    PPD_DEFINE,         //> Define a new macro
    PPD_IF,             //> C-like "#IF" operator
    PPD_ELSE,           //> C-like "#ELSE" operator
    PPD_ENDIF           //> C-like "#ENDIF" operator
};


enum ePreprocessorDirectiveType getPreprocessorTokenType(struct programToken_t* token);

//====================== Main Method ======================//

uint8_t preprocessor_run(struct programToken_t* head) {
    printf("\n[Preprocessor]: Starting");
    struct programToken_t* token = head;
    bool bIncludeToken = true; //Set to false if preprocessor directives dictate tokens between a
                               //preprocessor if-then statement should be removed from compilation

    bool bConditionalExecution = false; //Set to true if we're in an if or else condition driven by
                                        //preprocessor directives. Used to catch dangling endifs.
    uint16_t nestLevel = 0; /* If we are dealing with nested #preprocessor if/then statements, this
                             variable will track how deeply nested we are so we make sure to remove
                             everything between if-thens that preclude certain code from being compiled */


    while(token != NULL) {

        switch(token->type) {
        case ePROGTOKEN_PREPROCESSOR_DIRECTIVE:

            if(bIncludeToken == true) {
                switch(getPreprocessorTokenType(token)) {
                case PPD_INCLUDE:
                    //TODO
                    break;
                
                case PPD_DEFINE:
                    //TODO
                    break;
                
                case PPD_IF:
                    //TODO
                    break;
                
                case PPD_ELSE:
                    if(bConditionalExecution) {
                        bIncludeToken = false;
                    } else {
                        //TODO ERROR, ELSE WITH NO IF
                    }
                    break;
                
                case PPD_ENDIF:
                    if(bConditionalExecution) {
                        if(nestLevel == 0) {
                            bConditionalExecution = false;
                        } else {
                            nestLevel--;
                        }
                    } else {
                        //TODO ERROR, ENDIF WITH NO IF
                    }
                    break;
                }
            } else {
                switch(getPreprocessorTokenType(token)) {               
                case PPD_IF:
                    nestLevel++;
                    break;
                
                case PPD_ELSE:
                    if(nestLevel == 0) {
                        bIncludeToken = true;
                    }
                    break;
                
                case PPD_ENDIF:
                    if(nestLevel == 0) {
                        bIncludeToken = true;
                        bConditionalExecution = false;
                    } else {
                        nestLevel--;
                    }
                    break;

                default:
                    //Do nothing for non-conditional execution related ones
                    break;
                }
            }
            //Always remove the token
            token = tokenizer_removeToken(token);
            break;
        
        default:
            if(bIncludeToken == true) {
                token = token->next;
            } else {
                token = tokenizer_removeToken(token);
            }
        }
    }
    printf("\n[Preprocessor]: Complete!");
    return 0;
}


// uint8_t handleVariable(struct programToken_t* token) {
//     //Variable syntax:
//     //var <name> <bits> <address_to_store_in_RAM(optional)>
    
//     //NOTE: strtok WILL modify the string in the token, but that is OK as we are deleting the token anyways.
//     char* strtok_token = strtok(token->instruction_text, " ");

//     if(strtok_token == NULL) {
//         printf("\n\n[FATAL ERROR]: [Preprocessor]: Variable name not specified on line %d", token->lineNumber);
//     }
//     char* varName = (char*)calloc(strlen(strtok_token),sizeof(char));
//     strcpy(varName, strtok_token);

//     strtok_token = strtok(NULL, " ");
//     uint8_t errorCode;
//     if(strtok_token == NULL) {//Case: var <name> --> assume size is one bit.
//         errorCode = library_addVariable(varName, 1);

//     } else {
//         uint16_t numBits;
//         if(parser(strtok_token, &numBits) != 0) {
//             printf("\n[Preprocessor]: [FATAL ERROR]: Failed to parse meaning of \"%s\" from text \"%s\" on line %d",strtok_token, token->instruction_text, token->lineNumber);
//             return 1;
//         }
//         strtok_token = strtok(NULL, " ");

//         if(strtok_token == NULL) {//Case: var <name> <numBits>
//             errorCode = library_addVariable(varName, numBits);

//         } else {//Case: var <name> <numBits> <address>
//             uint16_t addr;
//             if(parser(strtok_token, &addr) != 0) {
//                 printf("\n[Preprocessor]: [FATAL ERROR]: Failed to parse meaning of \"%s\" from text \"%s\" on line %d",strtok_token, token->instruction_text, token->lineNumber);
//                 return 1;
//             }
//             errorCode = library_addVariableWithAddress(varName, numBits, addr);
//         }
//     }

//     switch(errorCode) {
//         case LIBRARY_STATUS__NAME_EXISTS:
//             printf("\n\n[FATAL ERROR]: [Preprocessor]: Variable '%s' (line %d) is was declared elsewhere!",varName, token->lineNumber+1);
//             return 1;
        
//         case LIBRARY_STATUS__SYNTAX_ERROR:
//             printf("\n\n[FATAL ERROR]: [Preprocessor]: Variable '%s' (line %d) does not pass syntax check!",varName, token->lineNumber+1);
//             return 1;
        
//         case LIBRARY_STATUS__ADDR_CONFLICT:
//             printf("\n\n[FATAL ERROR]: [Preprocessor]: Variable '%s' (line %d) overlaps with another variable's address space!",varName, token->lineNumber+1);
//             return 1;

//     }

//     free(varName);

// }


uint8_t handleLabel(struct programToken_t* token) {
    uint8_t errorCode = library_addLabel(token);
    if(errorCode == LIBRARY_STATUS__NAME_EXISTS) {
        return 1;
    }
    return 0;
}

enum ePreprocessorDirectiveType getPreprocessorTokenType(struct programToken_t* token) {
    //TODO implement
    return PPD_INCLUDE;
}
