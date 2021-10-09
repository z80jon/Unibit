#ifndef ASSEMBLER_H_
#define ASSEMBLER_H_

#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char *argv[]);

void assembler_assemble(char** lines, int32_t numLines);


#endif