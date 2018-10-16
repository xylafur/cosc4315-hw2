SOURCE=./src/
INCLUDE=./include/
OBJECTS=./objects/

LEXER=flex
LEX_DIR=./lexer/
LEX_INPUT=$(SOURCE)lexer.l
LEX_OUTPUT=$(LEX_DIR)lex.yy.c
LEX_OBJECT=$(OBJECTS)lex.o

PARSER=bison
PARSER_DIR=./parser/
PARSER_INPUT=$(SOURCE)parser.y
PARSER_OUTPUT=$(PARSER_DIR)parser.tab.c
PARSER_HEADER=$(INCLUDE)parser.tab.h
PARSER_DEBUG_FLAGS=-Wconflicts-sr --debug --report=all

MAIN=main.cpp
FILES=$(LEX_OBJECT) $(PARSER_OUTPUT) $(MAIN)
CC=g++
CC_FLAGS=-std=c++11

OUTPUT=mypython

mypython: lex_object
	$(CC) $(CC_FLAGS) -o $(OUTPUT) -I $(INCLUDE) $(FILES)

lex_object: objects parser lexer
	gcc -I $(INCLUDE) -c $(LEX_OUTPUT) -o $(LEX_OBJECT)

lexer:
	mkdir $(LEX_DIR)
	$(LEXER) -o $(LEX_OUTPUT) $(LEX_INPUT) 

parser:
	mkdir $(PARSER_DIR)
	$(PARSER) -o $(PARSER_OUTPUT) -d  $(PARSER_INPUT)
	mv $(PARSER_DIR)*.h $(INCLUDE)
	sed -i 's/yylex/get_next_token/g' $(PARSER_OUTPUT)

objects:
	mkdir objects

test: mypython
	python3 testing/tester.py


clean:
	rm -r $(PARSER_DIR) $(LEX_DIR) $(PARSER_HEADER) $(OBJECTS) $(OUTPUT)
