SOURCE=./src/
INCLUDE=./include/
OBJECTS_DIR=./objects/

PARSING_SOURCE=$(SOURCE)parsing/
UTIL_SOURCE=$(SOURCE)util/
EXECUTION_SOURCE=$(SOURCE)virtual_machine/

LEXER=flex
LEX_INPUT=$(PARSING_SOURCE)lexer.l
LEX_OUTPUT=$(PARSING_SOURCE)lex.yy.c
LEX_OBJECT=$(OBJECTS_DIR)lex.o

PARSER=bison
PARSER_INPUT=$(PARSING_SOURCE)parser.y
PARSER_OUTPUT=$(PARSING_SOURCE)parser.tab.c
PARSER_HEADER=$(INCLUDE)parser.tab.h
PARSER_DEBUG_FLAGS=-Wconflicts-sr --debug --report=all
PARSER_OBJECT=$(OBJECTS_DIR)parse.o

AST=$(PARSING_SOURCE)ast.cpp
AST_OBJECT=$(OBJECTS_DIR)ast.o

AST_UTIL_INPUT=$(PARSING_SOURCE)node_util.cpp
AST_UTIL_OBJECT=$(OBJECTS_DIR)node_util.o

#the order here matters.  We need some of the things that the parser generates
#before we can use it in the node util for instance
PARSING_OBJECTS=$(LEX_OBJECT) $(PARSER_OBJECT) $(AST_UTIL_OBJECT) $(AST_OBJECT)


GEN_UTIL=$(UTIL_SOURCE)gen_util.c
GEN_UTIL_OBJECT=$(OBJECTS_DIR)gen_util.o

IF_ELSE=$(UTIL_SOURCE)if_else_detector.cpp
IF_ELSE_OBJECT=$(OBJECTS_DIR)if_else_detector.o

MUTATION=$(UTIL_SOURCE)mutation_detector.cpp
MUTATION_OBJECT=$(OBJECTS_DIR)mutation_detector.o

UTIL_OBJECTS=$(GEN_UTIL_OBJECT) $(IF_ELSE_OBJECT) $(MUTATION_OBJECT)


VIRTUAL_MACHINE=$(EXECUTION_SOURCE)virtual_machine.cpp
VIRTUAL_MACHINE_OBJECT=$(OBJECTS_DIR)virtual_machine.o

BCODE_GEN=$(EXECUTION_SOURCE)bcode_generator.cpp
BCODE_GEN_OBJECT=$(OBJECTS_DIR)bcode_gen.o

EXECUTION_OBJECTS=$(VIRTUAL_MACHINE_OBJECT) $(BCODE_GEN_OBJECT)


MAIN=$(SOURCE)main.cpp
MAIN_OBJECT=$(OBJECTS_DIR)main.o

OBJECTS=$(PARSING_OBJECTS) $(UTIL_OBJECTS) $(MAIN_OBJECT) #$(EXECUTION_OBJECTS)

CC=g++
CC_FLAGS=-g -pedantic -std=c++11 -I $(INCLUDE)

OUTPUT=mypython

mypython: $(OBJECTS)
	$(CC) $(CC_FLAGS) -o $(OUTPUT) $(OBJECTS)

$(LEX_OBJECT): objects parser lexer
	gcc -I $(INCLUDE) -c $(LEX_OUTPUT) -o $(LEX_OBJECT)

$(GEN_UTIL_OBJECT): objects
	gcc -I $(INCLUDE) -c $(GEN_UTIL) -o $(GEN_UTIL_OBJECT)

$(IF_ELSE_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(IF_ELSE) -o $(IF_ELSE_OBJECT)

$(MUTATION_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(MUTATION) -o $(MUTATION_OBJECT)

$(PARSER_OBJECT): objects parser $(LEX_OBJECT)
	$(CC) $(CC_FLAGS) -c $(PARSER_OUTPUT) -o $(PARSER_OBJECT)

$(AST_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(AST) -o $(AST_OBJECT)

$(MAIN_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(MAIN) -o $(MAIN_OBJECT)

$(AST_UTIL_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(AST_UTIL_INPUT) -o $(AST_UTIL_OBJECT)

$(VIRTUAL_MACHINE_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(VIRTUAL_MACHINE) -o $(VIRTUAL_MACHINE_OBJECT)

$(BCODE_GEN_OBJECT): objects
	$(CC) $(CC_FLAGS) -c $(BCODE_GEN) -o $(BCODE_GEN_OBJECT)

lexer:
	$(LEXER) -o $(LEX_OUTPUT) $(LEX_INPUT)

parser:
	$(PARSER) -o $(PARSER_OUTPUT) -d  $(PARSER_INPUT)
	mv $(PARSING_SOURCE)*.h $(INCLUDE)
	sed -i 's/yylex/get_next_token/g' $(PARSER_OUTPUT)

objects:
	mkdir objects

test: mypython
	python3 testing/tester.py

clean:
	rm -r $(PARSER_HEADER) $(OBJECTS_DIR) $(OUTPUT) $(LEX_OUTPUT) $(PARSER_OUTPUT)
