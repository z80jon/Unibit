#include "assembler.h"
#include "tokenizer.h"
#include "parser.h"
#include "library.h"


struct programToken_t* head = NULL;

/**
 * \brief Step through each of the program tokens (starting from the head) and resolve
 *        each instruction's operand from text to a raw binary value
 */
void resolveOperands(void);

void assembler_run(char* inputFile, char* outputFile) {

    //1) Read in main file, while at the same time...
    //   >removing comments, extra whitespaces, and blank/empty lines
    //   >storing opcodes and labels in a linked list
    //   >removing (and parsing) preprocessor statements and acting on them accordingly
    //   >removing variable declarations and adding them to a library
    //   >adding labels to a library to be resolved later (once addresses are assigned)
    printf("\nTokenizing file...");
    head = tokenizer_tokenize(inputFile);//assembler_tokenizeText(linesOfText, numLines)3;
    printf("\nTokenization complete!");

    //if(preprocessor_run(head) != 0) {
    //    printf("\n[ERROR]: [Assembler]: Preprocessor failed!");
    //}

    //library_assignVariableAddresses();
    //library_ResolveLabelAddresses(head);

    /* Go through each instruction and translate its operand into raw binary data */
    resolveOperands();
                        

    //Step make the output file
    if(assembler_generateHex(head) != 0) {
        printf("\n[Assembler]: [Error]: Assembly failed!");
    }

    if(fileIO_generateIntelHexFile(head, "output_high.hex", "output_low.hex") != 0) {
    }

    library_freeMemory();

    struct programToken_t* pointer = head;
    struct programToken_t* pointer2;
    printf("\n\n==== Text as follows: ====\n\n");
    while(pointer != NULL) {
        tokenizer_printOutToken(pointer);
        // if(pointer->instruction_text != NULL) {
        //     free(pointer->instruction_text);
        // }
        pointer2 = pointer->next;
        free(pointer);
        pointer = pointer2;
    }

    printf("Assembly complete!\n");
}


uint8_t assembler_generateHex(struct programToken_t* head) {
    printf("\n[Hex Generation]: Starting...");
    struct programToken_t* token = head;

    while(token != NULL) {
        if(token->type == ePROGTOKEN_INSTRUCTION) {
            //if(parser_begin(token->instruction_text, &(token->romData)) != 0) {
            //    printf("\n[Hex Generation]: [Error]: Unable to parse operand \"%s\" on line %d!",token->instruction_text, token->lineNumber);
            //    return 1;
            //}

            // switch(token->opcode) {
            //     case LOAD:
            //         token->romData = token->romData | 0x0000;
            //         break;
            //     case STORE:
            //         token->romData = token->romData | 0x4000;
            //         break;
            //     case NEGATE:
            //         token->romData = token->romData | 0x8000;
            //         break;
            //     case JUMPIFZERO:
            //         token->romData = token->romData | 0xC000;
            //         break;
            //     case UNDEFINED:
            //         printf("\n[Hex Generation]: ERROR: Undefined opcode! Cannot translate!");
            //         //TODO add more useful error info
            //         return 1;
            // }
        }

        token = token->next;
        //printf("\n%d",token);
    }
    printf("\n[Hex Generation]: Complete!");
    return 0;
}

void resolveOperands(void) {
    struct programToken_t* token = head;

    while(token != NULL) {

        if(token->type == ePROGTOKEN_INSTRUCTION) {
            struct instruction_t* data = ((struct instruction_t*) token->data);
            if(parser_begin(data->text,&(data->operand))) {
                printf("\n[ERROR]: Could not make sense of operand text \"%s\"",data->text);
                printf(" in source text \"%s\"",token->originalText);
            }
        }

        token = token->next;
    }
}