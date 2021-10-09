Process:

1) Read in file
2) throw away comment lines and /r/n
4) create linked list instruction structs


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