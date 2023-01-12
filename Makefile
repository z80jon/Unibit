CC = gcc
#-Wfatal-errors will abort compilation on first error!
CFLAGS = -Wall -pedantic -std=c99 -g -Wfatal-errors
#-WError -pedantic 

#Source Directory
SDIR = src
SRC = assembler.c fileIO.c library.c parser.c preprocessor.c tokenizer.c
ODIR = build
OBJ = $(SRC:.c=.o)
EXEC = main validation

.DEFAULT: all


#==== EXECUTABLE FILES ====#
main: $(OBJ)
	$(CC) $(SDIR)\main.c -o assembler $(CFLAGS) $(ODIR)/**.o

validation:
#We discard const qualifiers, so suppress that warning
	$(CC) $(SDIR)\validation.c -o validation -Wno-discarded-qualifiers $(CFLAGS) $(ODIR)/**.o -Ilibs/munit

#==== OBJECT FILES ====#
assembler.o: 
	$(CC) -c $(SDIR)\assembler.c $(CFLAGS) -o $(ODIR)/$@

fileIO.o:
	$(CC) -c $(SDIR)\fileIO.c $(CFLAGS) -o $(ODIR)/$@

library.o:
	$(CC) -c $(SDIR)\library.c $(CFLAGS) -o $(ODIR)/$@

parser.o:
	$(CC) -c $(SDIR)\parser.c $(CFLAGS) -o $(ODIR)/$@

preprocessor.o:
	$(CC) -c $(SDIR)\preprocessor.c $(CFLAGS) -o $(ODIR)/$@

tokenizer.o:
	$(CC) -c $(SDIR)\tokenizer.c $(CFLAGS) -o $(ODIR)/$@


all: $(EXEC)



## $(EXEC): $(OBJ)
##	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LBLIBS)

clean:
	rm -rf $(ODIR)/**.o
