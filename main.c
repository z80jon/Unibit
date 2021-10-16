#include "main.h"

int main(int argc, char *argv[]) {
    if(argc < 2 || argc > 3) {
        printf("Error! needs at least 1 and at most 2 arguments: input file path, (optional): output file path");
        return 1;
    }

    if(!fileHandler_doesFileExist(argv[1])) {
        printf("ERROR: Input file \"%s\" does not exist!", argv[1]);
        return 1;
    }

    if(argc > 2 && fileHandler_doesFileExist(argv[2])) {
        printf("NOTE: output file file \"%s\" exists, and will be overwritten!", argv[2]);
        assembler_run(argv[1], argv[2]);
    } else {
        assembler_run(argv[1], "output.hex");
    }

    return 0;
}

