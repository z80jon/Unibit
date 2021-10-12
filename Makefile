CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -g

SRC = 
OBJ = $(SRC:.cc=.o)
EXEC = assembler

default: all

all: $(EXEC)



$(EXEC): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)