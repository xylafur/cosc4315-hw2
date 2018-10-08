LEXER=flex
LEX_DIR=./lexer/
LEX_INPUT=$(LEX_DIR)lexer.l
LEX_OUTPUT=$(LEX_DIR)lex.yy.c
INCLUDE=./include/

MAIN=main.cpp
FILES=$(LEX_OUTPUT) $(MAIN)
CC=g++

OUTPUT=out

lex:
	$(LEXER) -o $(LEX_OUTPUT) $(LEX_INPUT) 

main: lex
	$(CC) -o $(OUTPUT) -I $(INCLUDE) $(FILES)

clean:
	rm $(OUTPUT) $(LEX_OUTPUT)
