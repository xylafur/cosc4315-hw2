LEXER=flex
LEX_DIR=./lexer/
LEX_FILE=$(LEX_DIR)lexer.l
LEX_OUTPUT=$(LEX_DIR)lex.yy.c
INCLUDE=./include/

CC=gcc

OUTPUT=out

lex:
	$(LEXER) -o $(LEX_OUTPUT) $(LEX_FILE) 
	$(CC) -o $(OUTPUT) -I $(INCLUDE) $(LEX_OUTPUT)

clean:
	rm $(OUTPUT) $(LEX_OUTPUT)
