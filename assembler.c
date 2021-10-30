#include "assembler.h"




void assembler__run(char* inputFile, char* outputFile) {

    //1) Read in main file, while at the same time...
    //   >removing comments, extra whitespaces, and blank/empty lines
    //   >storing opcodes and labels in a linked list
    //   >removing (and parsing) preprocessor statements and acting on them accordingly
    //   >removing variable declarations and adding them to a library
    //   >adding labels to a library to be resolved later (once addresses are assigned)
    printf("\nTokenizing file...");
    head = tokenizer__tokenize(inputFile);//assembler_tokenizeText(linesOfText, numLines)3;
    printf("\nTokenization complete!");

    if(preprocessor__run(head) != 0) {
        printf("\n[FATAL ERROR]: [Assembler]: Preprocessor failed!");
    }

    library__assign_variable_addresses();
    library__resolve_label_addresses(head);

    //Step make the output file
    if(assembler__generate_hex(head) != 0) {
        printf("\n[Assembler]: [Error]: Assembly failed!");
    }

    if(fileHandler__output_hex_data(head, "output_high.hex", "output_low.hex") != 0) {
    }

    library__free_memory();

    struct program_token* pointer = head;
    struct program_token* pointer2;
    printf("\n\n==== Text as follows: ====\n\n");
    while(pointer != NULL) {

        tokenizer__print_out_token(pointer);
        if(pointer->instruction_text != NULL) {
            free(pointer->instruction_text);
        }
        pointer2 = pointer->nextToken;
        free(pointer);
        pointer = pointer2;
    }

    printf("Assembly complete!\n");
}


uint8_t assembler__generate_hex(struct program_token* head) {
    printf("\n[Hex Generation]: Starting...");
    struct program_token* token = head;

    while(token != NULL) {
        if(token->tokenType == PROGTOK__INSTRUCTION) {
            if(parser(token->instruction_text, &(token->romData)) != 0) {
                printf("\n[Hex Generation]: [Error]: Unable to parse operand \"%s\" on line %d!",token->instruction_text, token->lineNumber);
                return 1;
            }

            switch(token->opcode) {
                case LOAD:
                    token->romData = token->romData | 0x0000;
                    break;
                case STORE:
                    token->romData = token->romData | 0x4000;
                    break;
                case NEGATE:
                    token->romData = token->romData | 0x8000;
                    break;
                case JUMPIFZERO:
                    token->romData = token->romData | 0xC000;
                    break;
                case UNDEFINED:
                    printf("\n[Hex Generation]: ERROR: Undefined opcode! Cannot translate!");
                    //TODO add more useful error info
                    return 1;
            }
        }

        token = token->nextToken;
        //printf("\n%d",token);
    }
    printf("\n[Hex Generation]: Complete!");
    return 0;
}



