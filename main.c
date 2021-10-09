#include "main.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Error! needs 2 arguments: input file path, output file path");
        return 1;
    }

    if(!fileHandler_doesFileExist(argv[1])) {
        printf("ERROR: Input file \"%s\" does not exist!", argv[1]);
        return 1;
    }

    if(!fileHandler_doesFileExist(argv[2])) {
        printf("NOTE: output file file \"%s\" exists, and will be overwritten.", argv[2]);
        return 1;
    }

    assembler_assemble(argv[1], argv[2]);

    return 0;
}

