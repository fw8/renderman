#
#	Makefile f"ur den Shading-Compiler
#
#
# Eventuell m"ussen die Pfade f"ur BIN und INCL ge"andert werden !
#------------------------------------------------------------------------------
BIN = ../bin
#------------------------------------------------------------------------------
CFLAGS = -O
YACC = yacc 
LEX = lex
OBJ = \
sc.o error.o symtab.o tree.o gen.o init.o hash.o memory.o message.o misc.o\
sc-lex.o sc-parse.o

all: sc libsh.a
libsh.a:
	gensh
	mv libsh.a ../lib

sc: $(OBJ)
	$(CC) -o sc $(CFLAGS) $(OBJ) -ll
sc.o: sc.c
	$(CC) -c $(CFLAGS) sc.c
error.o: error.c
	$(CC) -c $(CFLAGS) error.c
symtab.o: symtab.c symtab.h tokens.h hash.h
	$(CC) -c $(CFLAGS) symtab.c
tree.o: tree.c tree.h
	$(CC) -c $(CFLAGS) tree.c
gen.o: gen.c gen.h symtab.h
	$(CC) -c $(CFLAGS) gen.c
init.o: init.c symtab.h tokens.h
	$(CC) -c $(CFLAGS) init.c
hash.o: hash.c hash.h
	$(CC) -c $(CFLAGS) hash.c
memory.o: memory.c
	$(CC) -c $(CFLAGS) memory.c
message.o: message.c
	$(CC) -c $(CFLAGS) message.c
misc.o: misc.c
	$(CC) -c $(CFLAGS) misc.c
sc-lex.o: sc-lex.c tokens.h
	$(CC) -c $(CFLAGS) sc-lex.c
sc-lex.c: sc-lex.l tokens.h
	$(LEX) sc-lex.l
	@mv lex.yy.c sc-lex.c
sc-parse.o: sc-parse.c symtab.h gen.h
	$(CC) -c $(CFLAGS) sc-parse.c
sc-parse.c tokens.h: sc-parse.y
#	@echo "expect 1 shift/reduce conflict (dangling else !)"
	$(YACC) -d sc-parse.y
	@mv y.tab.c sc-parse.c
	@mv y.tab.h tokens.h

install: sc libsh.a addsh delsh listsh
	cp sc addsh delsh listsh $(BIN)

clean:
	rm -f $(OBJ) sc sc-lex.c sc-parse.c core
