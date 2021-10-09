#include "main.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Error! needs 2 arguments: input file path, output file path");
        return 1;
    }

    // FILE *fp;
    // char** linesOfText;
    // uint32_t numLines;

    // for(uint32_t i = 0; i < numLines; i++) {
    //     printf("\nLine %d: \"%s\"",i,linesOfText[i]);
    // }
 
    // assembler_assemble(linesOfText, numLines);

    assembler_assemble(argv[1], argv[2]);

    return 0;
}