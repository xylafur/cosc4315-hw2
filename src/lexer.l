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
def                         return DEF;
return                      return RETURN;
print                       return PRINT;
if                          return IF;
else                        return ELSE;

for                         return FOR;
while                       return WHILE;

True                        return TRUE;
False                       return FALSE;

[0-9]*\.[0-9]+              yylval.real=atof(yytext); return REAL;
[0-9]+\.[0-9]*              yylval.real=atof(yytext); return REAL;
[0-9]+                      yylval.num=atoi(yytext); return NUMBER;

==                          return EQUALSEQUALS;
=                           return EQUALS;

:                           return COLON;
\*                          return STAR;
\+                          return PLUS;
-                           return MINUS;
\/                          return SLASH;
,                           return COMMA;

\(                          return LPARENTH;
\)                          return RPARENTH;

\\                          return BACKSLASH;

\"[a-zA-Z0-9_\!']*\"        yylval.str=strdup(yytext); return STRING;
\'[a-zA-Z0-9_\!"]*\'        yylval.str=strdup(yytext); return STRING;

[a-zA-Z][a-zA-Z0-9_]*       yylval.str=strdup(yytext); return IDENTIFIER;
#[^\n]+                     /*  Just consume    */
"   "                       return INDENT;
\n                          return NEWLINE;
" "                         /*return WHITESPACE;*/
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
