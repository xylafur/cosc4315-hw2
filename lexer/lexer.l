%{
#include <stdio.h>
#include "tokens.h"

int whitespace_matters = 1;

unsigned int global_indent_level = 0;
unsigned int dedent_count = 0;
int cached_token = 0;
int token = 0;
int first_in_line = 1;

int get_next_token(void);

%}

%%
def                         return DEF;
return                      return RETURN;
print                       return PRINT;
if                          return IF;
else                        return ELSE;

for                         return FOR;
while                       return WHILE;

[0-9]*\.[0-9]+              return REAL;
[0-9]+\.[0-9]*              return REAL;
[0-9]+                      return NUMBER;

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

\"[a-zA-Z0-9_\!']*\"        return STRING;
\'[a-zA-Z0-9_\!"]*\'        return STRING;

[a-zA-Z][a-zA-Z0-9_]*       return IDENTIFIER;
"   "                       return INDENT;
\n                          return NEWLINE;
" "                         {if(whitespace_matters) return WHITESPACE;}
.                           ;

%%

void print_type(int token)
{
    printf("%s", NAMES[token]);
}


int yywrap()
{
    return 1;
}


/*
 *  Take what we get from yylex and return the proper token based on indention
 *  level.  Adds tokens like dedent and ensures that we never indent more than
 *  once
 */
int get_next_token()
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

int main (int argc, char * argv[])
{
    if (argc != 2){
        printf("Need input file\n");
        return 1;
    }

    yyin=fopen(argv[1], "r");

    int token;
    token = get_next_token();
    while(token){
        printf(" ");
        print_type(token);
        printf(" ");
        token = get_next_token();
    }
    printf("\n");
}
