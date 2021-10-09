#include "assembler.h"
#include "fileHandler.h"
#include "fileHandler.c"
//test
int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Error! needs 2 arguments: input file path, output file path");
        return 1;
    }
    FILE *fp;
    char** linesOfText;
    uint32_t numLines;

    fileHandler_readInFile(argv[1],&linesOfText, &numLines);

    for(uint32_t i = 0; i < numLines; i++) {
        printf("\nLine %d: \"%s\"",i,linesOfText[i]);
    }
 
    assembler_assemble(linesOfText, numLines);

    return 0;
}

void assembler_assemble(char** lines, int32_t numLines) {
    printf("\n====ASSEMBLING %d LINES====\n\n",numLines);
     int32_t lineLength;

    for(int32_t i = 0; i < numLines; i++) {
        printf("Line %d: \"%s\"",i,lines[i]);
        switch(toupper(lines[i][0])) {
            case 'N':
                printf(" --> NGA");
                break;
            
            case 'L':
                lineLength = strlen(lines[i]);
                if(lineLength < 3)
                    printf("\n\nAssembler error: missing address to load from");
                printf(" --> LOAD ");
        }

        printf("\n");
    }
}

