LEXER=flex
LEX_FILE=lexer.l
LEX_OUTPUT=lex.yy.c
INCLUDE=./

CC=gcc

OUTPUT=out

lex:
	$(LEXER) $(LEX_FILE)
	$(CC) -o $(OUTPUT) -I $(INCLUDE) $(LEX_OUTPUT)

clean:
	rm $(OUTPUT) $(LEX_OUTPUT)
