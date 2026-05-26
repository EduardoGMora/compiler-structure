CC      = gcc
CFLAGS  = -Wall -Wno-unused-function

all: analyzer

analyzer: lex.yy.c parser.tab.c
	$(CC) $(CFLAGS) -o analyzer lex.yy.c parser.tab.c

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

clean:
	rm -f lex.yy.c parser.tab.c parser.tab.h analyzer

test-correct: analyzer
	./analyzer test-correct.c

test-incorrect: analyzer
	./analyzer test-incorrect.c
