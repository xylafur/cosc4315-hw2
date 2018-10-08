LEXER=flex
LEX_DIR=./lexer/
LEX_INPUT=$(LEX_DIR)lexer.l
LEX_OUTPUT=$(LEX_DIR)lex.yy.c
INCLUDE=./include/

PARSER=bison
PARSER_DIR=./parser/
PARSER_INPUT=$(PARSER_DIR)parser.y
PARSER_OUTPUT=$(PARSER_DIR)parser.tab.c

MAIN=main.cpp
FILES=$(LEX_OUTPUT) $(MAIN)
CC=g++

OUTPUT=out

lex:
	$(LEXER) -o $(LEX_OUTPUT) $(LEX_INPUT) 

parse:
	$(PARSER) -o $(PARSER_OUTPUT) $(PARSER_INPUT)

main: lex parse
	$(CC) -o $(OUTPUT) -I $(INCLUDE) $(FILES)

clean:
	rm $(OUTPUT) $(LEX_OUTPUT) $(PARSER_OUTPUT)
