%{
#include <stdio.h>
#include "tokens.h"

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

int main (int argc, char * argv[])
{
    if (argc != 2){
        printf("Need input file\n");
        return 1;
    }

    yyin=fopen(argv[1], "r");

    int token;
    token = yylex();
    while(token){
        printf(" ");
        print_type(token);
        printf(" ");
        token = yylex();
    }
    printf("\n");
}
