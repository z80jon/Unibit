CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -g
#-WError -pedantic 

SRC = 
OBJ = $(SRC:.cc=.o)
EXEC = main

.DEFAULT: all

main: fileIO library parser preprocessor assembler tokenizer
	$(CC) main.c -o assembler $(CFLAGS) *.o

assembler:
	$(CC) -c assembler.c $(CFLAGS)

fileIO:
	$(CC) -c fileIO.c $(CFLAGS)

library:
	$(CC) -c library.c $(CFLAGS)

parser:
	$(CC) -c parser.c $(CFLAGS)

preprocessor:
	$(CC) -c preprocessor.c $(CFLAGS)

tokenizer:
	$(CC) -c tokenizer.c $(CFLAGS)


all: $(EXEC)



## $(EXEC): $(OBJ)
##	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf *.o
