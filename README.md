Welcome to the Unibit assembler project! This is an effort to create a minimalist assembler for the UNIBIT CPU architecture.
I should publish a github repo with specifications of that architecture at some point, but anyhow, this is the repo for
a barebones assembler created to assist in the development of software/software libraries for the CPU. It is written more
with emphasis on ease of understanding and modularity/adaptability. If you're looking to write your own CPU assembler,
hopefully this will make a good starting point if you fork it.

The assembler is currently very early in development, and doesn't actually produce any executible code. It does, however,
produce meaningful output when the sampleProgram.txt demo file is passed as the input argument.

This assembler works in several steps, in the interest of simplicity of design, debugging, and implementation:

1) Read in file, throwing away comment lines and carriage returns. Store labels, opcodes, etc. in discrete entries in a linked list. Pre-Processor runs at this time, inserting any files in-line exactly where the include statement is found.

2) Populate a dictionary with the names of variables and labels
------If a variable has an explicit address in the code, it is resolved against other predefined address variables.

3) Determine where to place instructions in ROM

4) Determine where to place variables in RAM (if their address was not explicitly defined), scanning the file tokens and removing the variable declarations along the way.

4) address resolution of references to labels and variables

5) output file generation in Intel hex format

There are several key files in the assembler, each of which has a key task.

main.h/.c: 

fileHandler.h/.c: Handle reading in files and cleaning up the text in them, as well as generating Intel hex files

parser.h/.c: interprets math strings, recognizing references to the symbol library where relevant, and calculating values.

library.h/.c: Accepts symbol definitions and information for future recollection. Handles variable address resolution and
memory allocation.

tokenizer.h/.c: takes the sterilized text input and creates a linked list of tokens for processing

assembler.h/.c: fairly empty--just coordinates the other libraries to do stuff.

preprocessor.h/.c: handles label address resolution. Not much else at the moment.