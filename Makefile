SOURCE=./src/
INCLUDE=./include/
OBJECTS_DIR=./objects/

LEXER=flex
LEX_DIR=./lexer/
LEX_INPUT=$(SOURCE)lexer.l
LEX_OUTPUT=$(LEX_DIR)lex.yy.c
LEX_OBJECT=$(OBJECTS_DIR)lex.o

PARSER=bison
PARSER_DIR=./parser/
PARSER_INPUT=$(SOURCE)parser.y
PARSER_OUTPUT=$(PARSER_DIR)parser.tab.c
PARSER_HEADER=$(INCLUDE)parser.tab.h
PARSER_DEBUG_FLAGS=-Wconflicts-sr --debug --report=all
PARSER_OBJECT=$(OBJECTS_DIR)parse.o

AST=$(SOURCE)ast.cpp
AST_OBJECT=$(OBJECTS_DIR)ast.o

MAIN=$(SOURCE)main.cpp
MAIN_OBJECT=$(OBJECTS_DIR)main.o

OBJECTS=$(LEX_OBJECT) $(PARSER_OBJECT) $(AST_OBJECT) $(MAIN_OBJECT)

CC=g++
CC_FLAGS=-std=c++11 -I $(INCLUDE)

OUTPUT=mypython

mypython: $(OBJECTS)
	$(CC) $(CC_FLAGS) -o $(OUTPUT) $(OBJECTS)

$(LEX_OBJECT): objects parser lexer
	gcc $(CC_FLAGS) -c $(LEX_OUTPUT) -o $(LEX_OBJECT)

$(PARSER_OBJECT): objects parser $(LEX_OBJECT)
	$(CC) $(CC_FLAGS) -c $(PARSER_OUTPUT) -o $(PARSER_OBJECT)

$(AST_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(AST) -o $(AST_OBJECT)

$(MAIN_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(MAIN) -o $(MAIN_OBJECT)

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
	rm -r $(PARSER_DIR) $(LEX_DIR) $(PARSER_HEADER) $(OBJECTS_DIR) $(OUTPUT)
