%{
/*  C declarations  */
/*#include "tokens.h"*/
/*#include "lexer.h"*/
#include <stdio.h>
#include <string.h>

//we use this to allow us to compile lex as C code and then bison as c++
extern "C"
{
    int yylex(void);

    int yywrap()
    {
        return 1;
    }
}

int yyparse(void);

void yyerror(const char * str)
{
    fprintf(stderr, "Error: %s\n", str);
}

%}

%token DEF RETURN PRINT IF ELSE FOR WHILE TRUE FALSE
%token STRING NUMBER REAL IDENTIFIER
%token STAR PLUS SLASH MINUS EQUALSEQUALS EQUALS 
%token LPARENTH RPARENTH COMMA BACKSLASH COLON
%token INDENT DEDENT NEWLINE WHITESPACE

%union {
    long num;
    float real;
    char * str;
};

%type <str> IDENTIFIER
%type <num> NUMBER
%type <real> REAL

%%

program:    /*empty, but not sure why we need this*/
       |    program func
       |    program stmts
       ;

func:       DEF IDENTIFIER LPARENTH RPARENTH COLON NEWLINE
    {
        printf("Matched function: %s!\n", $2);
    }
    ;

stmts:      stmt
     |      stmts stmt
     ;

stmt:       PRINT NEWLINE
    |       NEWLINE
    {
        printf("Matched stmt!\n");
    }
    ;

%%

/*  additional C code   */
