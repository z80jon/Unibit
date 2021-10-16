#include "assembler.h"

void assembler_run(char* inputFile, char* outputFile) {

    //1) Read in file, throwing away comment lines and carriage returns. Store labels, preprocessor directives, opcodes, etc. in discrete entries in a linked list
    printf("Tokenizing file...\n");
    struct program_token* head = tokenizer_tokenize(inputFile);//assembler_tokenizeText(linesOfText, numLines)3;
    printf("Tokenization complete!\n");
    //Step 2b: Begin *ROM* address resolution (where in memory each line of code will go).

    //Step 3: Begin address resolution


    //2) Perform pre-assembly activities, which include:
    //------checking the syntax of each instruction/label/preprocessor directive/etc.
    //------expanding macros (if applicable) (tentative / planned feature)
    //------noting down labels, variables, etc. in a dictionary

    //3) Determine where to place instructions in ROM, and variables, in RAM.

    //4) address resolution of references to labels and variables

    //5) output file generation in Intel hex format
    


    //Step 7: Cleanup
    struct program_token* pointer = head;
    struct program_token* pointer2;
    printf("\n\n==== Text as follows: ====\n\n");
    while(pointer != NULL) {

        tokenizer_printOutToken(pointer);
        if(pointer->instruction_text != NULL) {
            free(pointer->instruction_text);
        }
        pointer2 = pointer->nextToken;
        free(pointer);
        pointer = pointer2;
    }

    printf("Assembly complete!\n");
}



