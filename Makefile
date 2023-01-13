CC = gcc
#C flags, test C flags
CFLAGS = -Wall -pedantic -std=c99 -g -Wfatal-errors
TCFLAGS = $(CFLAGS) -Wno-discarded-qualifiers -Ilibs/munit -I$(SDIR) -I$(TSDIR)
#-WError -pedantic 

#Source Directory
SDIR = src
#Test source directory
TSDIR = src\testcases
SRC = assembler.c fileIO.c library.c parser.c preprocessor.c tokenizer.c
ODIR = build
OBJ = $(SRC:.c=.o)
EXEC = main validation

.DEFAULT: all


#==== EXECUTABLE FILES ====#
main: $(OBJ)
	$(CC) $(SDIR)\main.c -o assembler $(CFLAGS) $(ODIR)/**.o

validation: parser_tests.o
#We discard const qualifiers, so suppress that warning
	$(CC) $(SDIR)\validation.c -o validation $(TCFLAGS) $(ODIR)/**.o

#==== VALIDATION TEST OBJECT FILES ====#
parser_tests.o:
	$(CC) -c $(TSDIR)\parser_tests.c $(TCFLAGS) -o $(ODIR)/$@

#==== ASSEMBLER OBJECT FILES ====#
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
