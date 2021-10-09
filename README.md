This assembler works in several steps, in the interest of simplicity of design, debugging, and implementation:

1) Read in file
2) throw away comment lines and /r/n
3) store every line of meaningful code (with extra spaces removed) in a linked list
4) preprocessor + variable creation, value assignment
5) binary generation
6) output file generation.




In an effort to keep things as simple as possible, only several files are used:

main.c:
    -handles interface with user
    -kicks off process by calling assembler_assemble

fileHandler.c: 
    -handles file reading and writing
    -library; can be told to fetch file contents (returns them nicely) or write serieses of tokens to a file

assembler.c:
    -orchestrates the actual assembly process.

dictionary.c:
    -handles remembering labels, variables, macros, etc.

datastructures.c:
    -handles any data structure-related functions having to do with tokens, etc.