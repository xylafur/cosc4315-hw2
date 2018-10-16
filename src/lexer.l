%{
#include <stdio.h>
//#include "tokens.h"
#include "parser.tab.h"
/*#include "lexer.h"*/

unsigned int global_indent_level = 0;
unsigned int dedent_count = 0;
int cached_token = 0;
int token = 0;
int first_in_line = 1;

%}

%%
def                         printf("Matched DEF\n"); return DEF;
return                      printf("Matched RETURN\n"); return RETURN;
print                       printf("Matched PRINT\n"); return PRINT;
if                          printf("Matched IF\n"); return IF;
else                        printf("Matched ELSE\n"); return ELSE;

for                         printf("Matched FOR\n"); return FOR;
while                       printf("Matched WHILE\n"); return WHILE;

True                        printf("Matched TRUE\n"); return TRUE;
False                       printf("Matched FALSE\n"); return FALSE;

and                         printf("Matched AND\n"); return AND;
or                          printf("Matched OR\n"); return OR;

[0-9]*\.[0-9]+              yylval.real=atof(yytext); printf("Matched REAL\n"); return REAL;
[0-9]+\.[0-9]*              yylval.real=atof(yytext); printf("Matched REAL\n"); return REAL;
[0-9]+                      yylval.num=atoi(yytext); printf("Matched NUMBER\n"); return NUMBER;

==                          printf("Matched EQUALSEQUALS\n"); return EQUALSEQUALS;
=                           printf("Matched EQUALS\n"); return EQUALS;

:                           printf("Matched COLON\n"); return COLON;
\*                          printf("Matched STAR\n"); return STAR;
\+                          printf("Matched PLUS\n"); return PLUS;
-                           printf("Matched MINUS\n"); return MINUS;
\/                          printf("Matched SLASH\n"); return SLASH;
,                           printf("Matched COMMA\n"); return COMMA;

\(                          printf("Matched LPARENTH\n"); return LPARENTH;
\)                          printf("Matched RPARENTH\n"); return RPARENTH;

\\                          printf("Matched BACKSLASH\n"); return BACKSLASH;

\"[^\"\n]*\"                yylval.str=strdup(yytext); return STRING;
\'[^\'\n]*\'                yylval.str=strdup(yytext); return STRING;

[a-zA-Z][a-zA-Z0-9_]*       yylval.str=strdup(yytext); printf("Matched IDENTIFIER\n"); return IDENTIFIER;
#[^\n]+                     /*  Just consume    */
"   "                       printf("Matched INDENT\n"); return INDENT;
\n                          printf("Matched NEWLINE\n"); return NEWLINE;
" "                         /*printf("Matched WHITESPACE\n"); return WHITESPACE;*/
.                           ;

%%

/*
 *  Take what we get from yylex and return the proper token based on indention
 *  level.  Adds tokens like dedent and ensures that we never indent more than
 *  once
 */
int _get_next_token()
{
    int current_indent = 0;
    //this is how we make up for multiple dedents
    if(dedent_count){
        dedent_count--;
        printf("Matched DEDENT!\n");
        return DEDENT;
    }
    //if we cahced a token previously, return that because we don't need a new
    //token
    if(cached_token){
        int ret = cached_token;
        cached_token = 0;
        
        return ret;
    }
    if(token == NEWLINE)
        first_in_line = 1;
    //continue getting tokens until we don't get whitespace
    token = yylex();
    //for an edge case, to keet the block going
    if(token == NEWLINE)
        return token;
    //if we get a newline character, we start a new line.  Indents are only
    //significant at the beginning of a line
    if(! first_in_line)
        //if this isn't the first entry in the line, we will just treat an
        //indent like a space
        return token != INDENT ? token : WHITESPACE;
    first_in_line = 0;

    while(token == INDENT){
        current_indent++;
        token = yylex();
    }

   //if the indention level is greater, that means that we are in a new
    //'block' and need to return a new indent token
    if(current_indent > global_indent_level){
        //I am assuming that the indention level can never be greater than 1 
        cached_token = token;
        global_indent_level = current_indent;
        return INDENT;
    } else if (current_indent < global_indent_level){
        //here we are dedenting, we can have more than one dedent, so we have
        //to keep track of this
        dedent_count = global_indent_level - current_indent - 1;
        global_indent_level = current_indent;
        cached_token = token;
        printf("Matched DEDENT\n");
        return DEDENT;
    }
    return token;
}

int get_next_token()
{
    int token;
    do{
        token = _get_next_token();
    } while(token == WHITESPACE);
    return token;
}

void set_input_file(char * str)
{
    yyin=fopen(str, "r");
}
