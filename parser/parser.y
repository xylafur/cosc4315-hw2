%{
/*  C declarations  */
#include "tokens.h"
#include "lexer.h"
%}

/*  bison declarations  */
%%

/*  grammar rules   */
program:    stmts     {printf("stmts\n");}
;

stmts:      stmt '\n' {printf("stmt");}
;

stmt:
; 
%%

/*  additional C code   */
