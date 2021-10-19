#include "assembler.h"

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

    if(preprocessor_run(head) != 0) {
        printf("[FATAL ERROR]: [Assembler]: Preprocessor failed!\n");
    }

    library_assignVariableAddresses();

    library_resolveLabelAddresses(head);

    //Step 7: Cleanup

    library__free_memory();

    struct program_token* pointer = head;
    struct program_token* pointer2;
    printf("\n\n==== Text as follows: ====\n\n");
    while(pointer != NULL) {

        //tokenizer_printOutToken(pointer);
        if(pointer->instruction_text != NULL) {
            free(pointer->instruction_text);
        }
        pointer2 = pointer->nextToken;
        free(pointer);
        pointer = pointer2;
    }

    printf("Assembly complete!\n");
}

