SOURCE=./src/
INCLUDE=./include/
OBJECTS_DIR=./objects/

LEXER=flex
LEX_INPUT=$(SOURCE)lexer.l
LEX_OUTPUT=$(SOURCE)lex.yy.c
LEX_OBJECT=$(OBJECTS_DIR)lex.o

PARSER=bison
PARSER_INPUT=$(SOURCE)parser.y
PARSER_OUTPUT=$(SOURCE)parser.tab.c
PARSER_HEADER=$(INCLUDE)parser.tab.h
PARSER_DEBUG_FLAGS=-Wconflicts-sr --debug --report=all
PARSER_OBJECT=$(OBJECTS_DIR)parse.o

UTIL_INPUT=$(SOURCE)node_util.cpp
UTIL_OBJECT=$(OBJECTS_DIR)node_util.o

AST=$(SOURCE)ast.cpp
AST_OBJECT=$(OBJECTS_DIR)ast.o

GEN_UTIL=$(SOURCE)gen_util.c
GEN_UTIL_OBJECT=$(OBJECTS_DIR)gen_util.o

MAIN=$(SOURCE)main.cpp
MAIN_OBJECT=$(OBJECTS_DIR)main.o

OBJECTS=$(LEX_OBJECT) $(PARSER_OBJECT) $(AST_OBJECT) $(MAIN_OBJECT) $(UTIL_OBJECT) $(GEN_UTIL_OBJECT)

CC=g++
CC_FLAGS=-std=c++11 -I $(INCLUDE)

OUTPUT=mypython

mypython: $(OBJECTS)
	$(CC) $(CC_FLAGS) -o $(OUTPUT) $(OBJECTS)

$(LEX_OBJECT): objects parser lexer
	gcc -I $(INCLUDE) -c $(LEX_OUTPUT) -o $(LEX_OBJECT)

$(GEN_UTIL_OBJECT): objects
	gcc -I $(INCLUDE) -c $(GEN_UTIL) -o $(GEN_UTIL_OBJECT)

$(PARSER_OBJECT): objects parser $(LEX_OBJECT)
	$(CC) $(CC_FLAGS) -c $(PARSER_OUTPUT) -o $(PARSER_OBJECT)

$(AST_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(AST) -o $(AST_OBJECT)

$(MAIN_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(MAIN) -o $(MAIN_OBJECT)

$(UTIL_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(UTIL_INPUT) -o $(UTIL_OBJECT)

lexer:
	$(LEXER) -o $(LEX_OUTPUT) $(LEX_INPUT) 

parser:
	$(PARSER) -o $(PARSER_OUTPUT) -d  $(PARSER_INPUT)
	mv $(SOURCE)*.h $(INCLUDE)
	sed -i 's/yylex/get_next_token/g' $(PARSER_OUTPUT)

objects:
	mkdir objects

test: mypython
	python3 testing/tester.py

clean:
	rm -r $(PARSER_HEADER) $(OBJECTS_DIR) $(OUTPUT) $(LEX_OUTPUT) $(PARSER_OUTPUT)
