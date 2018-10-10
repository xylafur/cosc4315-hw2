SOURCE=./src/

LEXER=flex
LEX_DIR=./lexer/
LEX_INPUT=$(SOURCE)lexer.l
LEX_OUTPUT=$(LEX_DIR)lex.yy.c

PARSER=bison
PARSER_DIR=./parser/
PARSER_INPUT=$(SOURCE)parser.y
PARSER_OUTPUT=$(PARSER_DIR)parser.tab.c

MAIN=main.cpp
FILES=$(LEX_OUTPUT) $(MAIN)
INCLUDE=./include/
CC=g++

OUTPUT=mypython

main: mypython
	echo "Built as $(OUTPUT)"

mypython: lexer parser
	$(CC) -o $(OUTPUT) -I $(INCLUDE) $(FILES)

lexer:
	mkdir $(LEX_DIR)
	$(LEXER) -o $(LEX_OUTPUT) $(LEX_INPUT) 

parser:
	mkdir $(PARSER_DIR)
	$(PARSER) -o $(PARSER_OUTPUT) $(PARSER_INPUT)
	sed -i 's/yylex/get_next_token/g' $(PARSER_OUTPUT)

clean:
	rm $(OUTPUT)
	rm -r $(PARSER_DIR) $(LEX_DIR)
