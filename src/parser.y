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

branch_condition:   expr1
                |   LPARENTH expr1 RPARENTH
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
     |  expr1 
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

expr1: expr1 OR expr2 
     | expr2 
     {
        printf("Matched expr1 [OR]\n");
     }
     ;

expr2: expr2 AND expr3 
     | expr3
     {
        printf("Matched expr2 [AND]\n");
     }
     ;

expr3: expr4 EQUALSEQUALS expr4
     | expr4
     {
        printf("Matched expr3 [==]\n");
     }
     ;

expr4:  expr4 PLUS expr5
     |  expr4 MINUS expr5
     |  expr5
     {

        printf("Matched expr4 [+ or -]\n");
     }
     ;

expr5:  expr5 STAR expr6
     |  expr5 SLASH expr6
     |  expr6
     {

        printf("Matched expr5 [* or /]\n");
     }
     ;

expr6:  PLUS expr6
     |  MINUS expr6
     |  func_call
     |  bool
     |  IDENTIFIER
     |  NUMBER
     |  REAL
     {

        printf("Matched expr6 [func_call or ident or number or bool ...] \n");
     }
     ;
bool:   TRUE
    |   FALSE
    {

        printf("Matched bool\n");
    }
    ;


%%

/*  additional C code   */
