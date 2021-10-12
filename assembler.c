#include "assembler.h"

void assembler_assemble(char* inputFile, char* outputFile) {

    //Step 2a: Break the file into managable tokens of text. Create dictionary entries and fill in opcode enums where possible.
    struct program_token* head = tokenizer_tokenize(inputFile);//assembler_tokenizeText(linesOfText, numLines);
    
    //Step 2b: Begin *ROM* address resolution (where in memory each line of code will go).

    //Step 3: Begin address resolution




    //Step 7: Cleanup
    struct program_token* pointer = head;
    struct program_token* pointer2;
    printf("\n\n==== Text as follows: ====\n\n");
    while(pointer != NULL) {

        if(pointer->instruction_text != NULL) {
            printf("%s\n",pointer->instruction_text);
            free(pointer->instruction_text);
        }
        pointer2 = pointer->nextToken;
        free(pointer);
        pointer = pointer2;
    }

    printf("Assembly complete!\n");
}



