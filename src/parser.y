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

%token DEF RETURN PRINT IF ELSE FOR WHILE TRUE FALSE AND OR
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

program:    %empty
       |    program stmt
       {
            printf("Matched program\n");
       }
       ;


stmt:   simple_stmt
    |   compound_stmt
    {
        printf("Matched stmt\n");
    }
    ;

simple_stmt:    single_stmt NEWLINE
           {
                printf("matched simple_stmt\n");
           }
           ;

mult_stmts: %empty 
          | mult_stmts simple_stmt
          | mult_stmts compound_stmt
          {
            printf("Found multiple simple stmts!\n");
          }
          ;

single_stmt:    %empty
           |    print_stmt
           |    assignment
           |    value_list
           |    return_stmt
           {
               printf("Matched single_stmt!\n");
           }
           ;

compound_stmt:  branch_stmt
             |  func_def
             {
                printf("Matched compound stmt\n");
             }
             ;

func_def:   DEF IDENTIFIER LPARENTH RPARENTH COLON block_stmt
        {
            printf("Matched function: %s!\n", $2);
        }
        ;

branch_condition:   bool_expr
                |   LPARENTH bool_expr RPARENTH
                {
                    printf("Found branch condition!\n");
                }
                ;

branch_stmt:    IF branch_condition COLON block_stmt
           |    IF branch_condition COLON block_stmt ELSE COLON block_stmt
           {
                printf("Matched branch_stmt\n");
           }
           ;

block_stmt: NEWLINE INDENT mult_stmts DEDENT
          {
            printf("Matched block_stmt\n");
          }
          ;

print_stmt: PRINT LPARENTH value_list RPARENTH
          {printf("Matched print stmt\n");}
          ;

assignment: IDENTIFIER EQUALS value
          {
            printf("Matched assignment\n");
          }
          ;

value:  STRING
     |  bool_expr
     {
        printf("Found value\n");
     }
     ;

value_list: value_list COMMA value
          | value
          {
            printf("Found value list\n");
          }
          ;

func_call:  IDENTIFIER LPARENTH RPARENTH
         {
            printf("funciton call!\n");
         }
         ;

return_stmt:    RETURN value
           {
            printf("Found return stmt\n");
           }
           ;

bool_expr:  bool_expr OR bool_expr2
         |  bool_expr2
         {
            printf("Matched bool_expr\n");
         }
         ;

bool_expr2: bool_expr2 AND bool_expr3
          | bool_expr3
          {
            printf("Matched bool_expr2\n");
          }
          ;
bool_expr3: bool EQUALSEQUALS bool
          | bool
          {
            printf("Matched bool_expr3\n");
          }
          ;

bool:   TRUE
    |   FALSE
    |   expr1
    {

        printf("Matched bool\n");
    }
    ;

expr1:  expr1 PLUS expr2
     |  expr1 MINUS expr2
     |  expr2
     {

        printf("Matched expr1\n");
     }
     ;

expr2:  expr2 STAR expr3
     |  expr2 SLASH expr3
     |  expr3
     {

        printf("Matched expr2\n");
     }
     ;

expr3:  PLUS expr3
     |  MINUS expr3
     |  func_call
     |  IDENTIFIER
     |  NUMBER
     |  REAL
     {

        printf("Matched expr3\n");
     }
     ;
%%

/*  additional C code   */
