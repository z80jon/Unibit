#include "main.h"

int main(int argc, char *argv[]) {
    if(argc < 2 || argc > 3) {
        printf("\nError! needs at least 1 and at most 2 arguments: input file path, (optional): output file path");
        return 1;
    }

    if(!fileHandler__does_file_exist(argv[1])) {
        printf("\nERROR: Input file \"%s\" does not exist!", argv[1]);
        return 1;
    }

    if(argc == 3) {
        if(fileHandler__does_file_exist(argv[2]))
            printf("\nNOTE: output file file \"%s\" exists, and will be overwritten!", argv[2]);
        assembler__run(argv[1], argv[2]);
    } else {
        printf("\nNOTE: Output will be placed in \"output.hex\". If it already exists, it will be overwritten.");
        assembler__run(argv[1], "output.hex");
    }

    return 0;
}

