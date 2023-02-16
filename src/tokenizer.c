#include "fileIO.h"
#include "tokenizer.h"
#include "datastructures.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//====================== Local Variables ======================//

const char* OPCODE_LIST_NEGATIVE[] = {"n", "negate", "invert"};
const char* OPCODE_LIST_LOAD[] = {"l", "ld", "load"};
const char* OPCODE_LIST_STORE[] = {"s", "st", "store"};
const char* OPCODE_LIST_JUMPIFZERO[] = {"j", "jiz", "jumpifzero"};

//====================== Local Functions ======================//

/**
 * \brief Given an input string, detect the type of token present and return it
 * 
 * \param c input string
 * \return eTokenType the type of token detected in the string
 */
enum eProgTokenType getTokenType(char* c); 


/**
 * @brief Given a string with a preprocessor directive in it, create a new token containing the directive and return it.
 * 
 * @param string the string with the preprocessor directive
 * @return struct programToken_t* a pointer to a new calloc'd programToken_t configured as a preprocessor token
 */
struct programToken_t* makePreprocessorToken(char* string, char* originalText);


/**
 * @brief Given a string with a label in it, create a new token containing the label and return it.
 * 
 * @param string the string with a label at the start of it
 * @return struct programToken_t* a pointer to a new calloc'd programToken_t configured as a label token
 */
struct programToken_t* makeLabelToken(char* string, char* originalText);




/**
 * \brief Given a string with an opcode in it, create a new token containing the opcode and return it.
 * 
 * \param inputText the string with an opcode at the start of it
 * \param originalText The original text (uncleaned of comments and such)
 * \return struct programToken_t*  a pointer to a new calloc'd programToken_t configured as an opcode token
 */
struct programToken_t* makeInstructionToken(char* inputText, char* originalText);


/**
 * @brief Given a string with a variable declaration inb it, create a new token containing the variable info and return it.
 * 
 * @param string the string with the variable declaration at the start of it
 * @return struct programToken_t* a pointer to a new calloc'd programToken_t configured as a variable declaration token.
 */
struct programToken_t* makeVariableToken(char* string);


/**
 * @brief Creates a generic token containing instruction_text and returns it. Handles malloc'ing and assigning default values automatically.
 * 
 * @param instruction_text the text to store in token->instruction_text
 * @param tokenType the programTokenType enum to store within the token
 * @param originalText The original text (uncleaned of comments and such)
 * @return struct programToken_t* the newly created token
 */
struct programToken_t* makeGenericToken(enum eProgTokenType type, char* originalText);

enum opCode identifyOpcode(char* text);


//====================== Main Method ======================//

struct programToken_t* tokenizer_tokenize(char* inputFile) {
    char* fileText;
    fileIO_readInFile(inputFile, &fileText);
    int index; //Pointer to beginning of text token
    //TODO error detection and handling

    //Case: Empty file
    if(strlen(fileText) == 0)
        return NULL;

    struct programToken_t* head = NULL;
    struct programToken_t* pointer = NULL;
    struct programToken_t* next = NULL;
    printf("File read");

    //Step 1: create linked list of tokens of each part of the program (instructions, labels, constants, etc.)
    while(1) {

        
        if(/*sterilizedLineText != NULL*/0) {

            switch(getTokenType(0/*sterilizedLineText*/)) {
//            case ePROGTOKEN_PREPROCESSOR_DIRECTIVE:
//                next = makePreprocessorToken(sterilizedLineText);
//                break;

//            case ePROGTOKEN_LABEL:
//                next = makeLabelToken(sterilizedLineText);
//                break;

            case ePROGTOKEN_INSTRUCTION:
                //next = makeInstructionToken(sterilizedLineText, linesOfText[currentLine]);
                
                break;

//            case eTOKENTYPE_VARIABLE:
//                next = makeVariableToken(sterilizedLineText);
//                break;

//            case eTOKENTYPE_UNKOWN:
//            default:
//                printf("Unknown token \"%s\" on line %d!",linesOfText[currentLine], currentLine);
//                //TODO bail out and free everything
//                break;

            default:
                break;
            }

            //Update token references & handle first token edge case
            if(head == NULL) {
                head = next;
                head->prev = NULL;
            } else {
                next->prev = pointer;
                pointer->next = next;
            }
            pointer = next;
        }
    
    }

    //Last token edge case
    pointer->next = NULL;

    //deallocate memory for the file text we just finished processing
    free(fileText);

    return head;
}




//====================== Helper Functions ======================//

// struct programToken_t* makePreprocessorToken(char* string) {
//     struct programToken_t* toReturn = makeGenericToken(string, eTOKEN_PREPROC_DIR);
//     return toReturn;
// }

struct programToken_t* makeLabelToken(char* label, char* originalText) {

    //Make token and allocate space for the label
    struct programToken_t* toReturn = makeGenericToken(ePROGTOKEN_LABEL, originalText);
    toReturn->data = (void*) calloc(1, sizeof(struct label_t));

    //Isolate just the label text using strtok and store it in 'buff'
    char* buff = (char*)calloc(strlen(label)+1,sizeof(char));
    strcpy(buff, label);
    buff = strtok(buff, ":");

    //Make the label_t structure and populate it with just the label text
    struct label_t* t = (struct label_t*)calloc(1, sizeof(struct label_t));
    t->text = (char*)calloc(strlen(buff),sizeof(char));
    strcpy(t->text, buff);

    free(buff);
    //toReturn->data = (void*)t;
    return toReturn;
}


struct programToken_t* makeInstructionToken(char* inputText, char* originalText) {

    //Make token and allocate space for the instruction
    struct programToken_t* toReturn = makeGenericToken(ePROGTOKEN_INSTRUCTION, inputText);
    toReturn->data = calloc(1, sizeof(struct instruction_t));
    struct instruction_t* data = ((struct instruction_t*) toReturn->data);

    data->opcode = identifyOpcode(inputText);

    //Extract the operand text
    data->text = fileIO_extractAndCleanOperand(inputText);

    return toReturn;
}


// struct programToken_t* makeVariableToken(char* string) {
//     struct programToken_t* toReturn = makeGenericToken(&(string[4]), eTOKEN_VARIABLE_DECLARATION);
    
//     return toReturn;
// }


struct programToken_t* makeGenericToken(enum eProgTokenType type, char* originalText) {
    struct programToken_t* toReturn = (struct programToken_t*)calloc(1, sizeof(struct programToken_t));
    toReturn->next = NULL;
    toReturn->type = type;

    //Copy over the originally input text
    toReturn->originalText = (char*)calloc(strlen(originalText)+1,sizeof(char));
    strcpy(toReturn->originalText, originalText);

    return toReturn;
}


struct programToken_t* tokenizer_removeToken(struct programToken_t* token) {
    
    //Interpret and de-allocate memory based on the type of token
    switch(token->type) {
        case ePROGTOKEN_INSTRUCTION:
            free(((struct instruction_t*)token->data)->text);
            break;

        case ePROGTOKEN_LABEL:
            free(((struct label_t*)token->data)->text);
            break;

        default:
            //NYI
            break;
    }

    //Update previous token if there is one
    if(token->prev != NULL) {
        token->prev->next = token->next;
    }

    if(token->next != NULL) {
        token->next->prev = token->prev;
    }
    
    struct programToken_t* toReturn = token->next;
    free(token);
    return toReturn;
}



//====================== String-Utility Functions ======================//

enum eProgTokenType getTokenType(char* c) {

    uint8_t index = 0;

    //Test for label
    while(c[index] != '\0' && c[index] != ' ' && c[index] != '\r' && c[index] != '\n' && index < strlen(c)) {
        if(c[index++] == ':')
            return ePROGTOKEN_LABEL;
    }

    //Test for variable
    //if(strstr(c, "var") == c)
    //    return eTOKENTYPE_VARIABLE;
    
    //No known token detected
    //return eTOKENTYPE_UNKOWN;

    if(identifyOpcode(c) != eINVALID) {
        return ePROGTOKEN_INSTRUCTION;
    }
    
    return -1;
}

enum opCode identifyOpcode(char* text) {
    enum opCode opcode = eINVALID;

    for(int i = 0; i < 3; i++) {
        if(fileIO_containsIgnoreCase(text,OPCODE_LIST_LOAD[i])) {
            opcode = eLOAD;
        } else if(fileIO_containsIgnoreCase(text,OPCODE_LIST_NEGATIVE[i])) {
            opcode = eNEGATE;
        } else if(fileIO_containsIgnoreCase(text,OPCODE_LIST_JUMPIFZERO[i])) {
            opcode = eJUMPIFZERO;
        } else if(fileIO_containsIgnoreCase(text,OPCODE_LIST_STORE[i])) {
            opcode = eSTORE;
        }
    }

    return opcode;
}

void tokenizer_printOutToken(struct programToken_t* t) {
    struct instruction_t* data;

    if(t == NULL)
        return;

    switch(t->type) {

        case ePROGTOKEN_INSTRUCTION:
            printf("[INSTRUCTION token: ");
            data = (struct instruction_t*) t->data;

            switch(data->opcode) {
                case eLOAD:
                    printf("LOAD from %d/0x%X, ",data->operand & 0x3FFF, data->operand & 0x3FFF);
                    break;
                case eNEGATE:
                    printf("NEGATE (with dangling address %d/0x%X), ",data->operand & 0x3FFF, data->operand & 0x3FFF);
                    break;
                case eSTORE:
                    printf("STORE to %d/0x%X, ",data->operand & 0x3FFF, data->operand & 0x3FFF);
                    break;
                case eJUMPIFZERO:
                    printf("JUMP to %d/0x%X, ",data->operand & 0x3FFF , data->operand & 0x3FFF);
                    break;
                case eINVALID:
                default:
                    printf("Invalid opcode. This should never happen regardless of input text!");
                    break;
                }
            printf("(operand text:\"%s\")]",data->text);
            break;

        case ePROGTOKEN_LABEL:
            //printf("[LABEL token: line#: %d, text: '%s']\n",t->lineNumber+1,t->instruction_text);
            break;
        
        // case eTOKEN_PREPROC_DIR:
        //     printf("[PREPROCESSOR DIRECTIVE token: line#: %d, text: '%s']\n",t->lineNumber+1,t->instruction_text);
        //     break;

        // case eTOKEN_VARIABLE_DECLARATION:
        //      printf("[VARIABLE DECLARATION token: line#: %d, text: '%s']\n",t->lineNumber+1,t->instruction_text);
        //      break;
        
        default:
            //printf("[UNKNOWN token: line#: %d, text: '%s']\n",t->lineNumber+1,t->instruction_text);
            break;
    }
    
}



int nextTokenPointer;
char* nextTokenText;
char* getNextToken(char* text, int* line) {
    //Base case
    if(text != NULL) {
        nextTokenText = text;
        nextTokenPointer = 0;
    }

    //TODO error handling, illegal tokens

    //First, find the beginning of the next token
    while(getTokenType(&(text[nextTokenPointer])) == -1) {

        //If we detect a comment, advance to the end of it
        if(bIsStartOfComment(text, nextTokenPointer)) {
            nextTokenPointer = findEndOfComment(text, nextTokenPointer);
        } else {
            nextTokenPointer++;
        }

        //Edge case: no additional tokens found
        if(text[nextTokenPointer] == '\0')
            return NULL;
    }

    int endOfToken = nextTokenPointer;

    //Based on the type of token, the end delineation may vary.
    switch(getTokenType(&(text[nextTokenPointer]))) {
    
    case ePROGTOKEN_VARIABLE:
    case ePROGTOKEN_INSTRUCTION:
        while(1) {
            if(text[endOfToken] == ';')
                break;
            
            //Skip past any comments we find along the way
            if(bIsStartOfComment(text, endOfToken)) {
                endOfToken = findEndOfComment(text, endOfToken);
            } else {
                endOfToken++;
            }
        }
        break;
        //End is marked by a ; not inside a comment
        
    case ePROGTOKEN_LABEL:
        //End is marked by a : immediately following it
        //No need to handle edge cases, comments, etc. as all labels are immediately followed by a ':'.
        while(text[++endOfToken] != ':');
        break;
    
    case ePROGTOKEN_PREPROCESSOR_DIRECTIVE:
        //End is marked by a newline without a '\' immediately preceding it (and not inside a comment)
        while(1) {

            if(text[endOfToken] == '\n' && text[endOfToken-1] != '\\')
                break;
            
            //If we start a comment
            if(bIsStartOfComment(text, endOfToken)) {
                //TODO fix functionality and how we handle preprocessor
                //ends of lines when there's a comment involved
                endOfToken = findEndOfComment(text, endOfToken);
            } else {
                endOfToken++;
            }
        }
        break;
    }

    //Copy over the token in full
    char* nextToken = calloc((endOfToken - nextTokenPointer + 1),sizeof(char));
    memcpy(nextToken, &(text[nextTokenPointer]), endOfToken-nextTokenPointer);

    nextTokenPointer = endOfToken;
    return nextToken;

}


