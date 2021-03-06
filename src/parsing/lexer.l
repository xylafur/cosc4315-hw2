%{
#include <stdio.h>
#include "parser.tab.h"
#include "gen_util.h"
#include <time.h>
#include <assert.h>

unsigned int global_indent_level = 0;
unsigned int dedent_count = 0;
int cached_token = 0;
int token = 0;
int first_in_line = 1;

/* this is used to keep track of groupings of stmts that corespond to a
 * particular block
 */
#define MAX_STMT_STACK_SIZE 1024
int stmt_stack [MAX_STMT_STACK_SIZE];
int num_stmts = 0;
int stmt_stack_pos = 0;

extern int orphan_else;

#define put_global() printf("Global: %d\n", global_indent_level)
#define put_current() printf("Current: %d\n", current_indent_level)

char delayed_block = 0;

//helper functions that manipulate the stmt stack.  SUpposed to represent the
//scope of a block
void enter_block()
{
    stmt_stack[stmt_stack_pos++] = num_stmts;
    num_stmts = 0;
}

void exit_block()
{
    if(stmt_stack_pos == 0){
        num_stmts = 0;
    }else{
        stmt_stack[stmt_stack_pos] = 0;
        num_stmts = stmt_stack[--stmt_stack_pos];
    }
}

#define print_token(token) switch(token){                                   \
case DEF: printf("DEF ");break; case RETURN: printf("RETURN ");break;       \
case PRINT: printf("PRINT ");break;case IF: printf("IF ");break;            \
case ELSE: printf("ELSE ");break; case FOR: printf("FOR ");break;           \
case WHILE: printf("WHILE ");break; case TRUE: printf("TRUE ");break;       \
case FALSE: printf("FALSE ");break; case AND: printf("AND ");break;         \
case OR: printf("OR ");break; case STRING: printf("STRING ");break;         \
case NUMBER: printf("NUMBER ");break; case REAL: printf("REAL ");break;     \
case IDENTIFIER: printf("IDENTIFIER ");break;                               \
case STAR: printf("STAR ");break;                                           \
case PLUS: printf("PLUS ");break; case SLASH: printf("SLASH ");break;       \
case MINUS: printf("MINUS ");break; case EQUALS: printf("EQUALS ");break;   \
case SINGLE: printf("SINGLE ");break;                                       \
case EQUALSEQUALS: printf("EQUALSEQUALS ");break;                           \
case GREATEREQUALS: printf("GREATEREQUALS ");break;                         \
case LESSEQUALS: printf("LESSEQUALS ");break;                               \
case GREATER: printf("GREATER ");break; case LESS: printf("LESS ");break;   \
case LPARENTH: printf("LPARENTH ");break;                                   \
case RPARENTH: printf("RPARENTH ");break;                                   \
case COMMA: printf("COMMA ");break;                                         \
case BACKSLASH: printf("BACKSLASH ");break;                                 \
case COLON: printf("COLON ");break; case INDENT: printf("INDENT ");break;   \
case DEDENT: printf("DEDENT");break; case NEWLINE: printf("NEWLINE ");break;\
case WHITESPACE: printf("WHITESPACE ");break;                               \
case 0: printf("NULL TOKEN!");break;                                      \
default: printf("Invalid token\n");break;}

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

and                         return AND;
or                          return OR;

[0-9]*\.[0-9]+              yylval.real=atof(yytext); return REAL;
[0-9]+\.[0-9]*              yylval.real=atof(yytext); return REAL;
[0-9]+                      {
                                /* the literal struct was going to be placed
                                 * inside a global structure which would be
                                 * used by the converter
                                 */
                                yylval.num=atoi(yytext);
                                struct literal lit = {.type = 0};
                                lit.value.n_val = yylval.num;
                                add_literal(lit);
                                return NUMBER;
                            }

==                          return EQUALSEQUALS;
\>=                          return GREATEREQUALS;
\<=                          return LESSEQUALS;
\>                           return GREATER;
\<                           return LESS;
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

\"[^\"\n]*\"                {   
                                //sadly no escape sequence support
                                yylval.str=strdup(yytext);
                                strcpy(yylval.str, yylval.str + 1);
                                int ii, len = strlen(yylval.str);
                                //we strip out all of the " characters
                                for(ii = 0; ii < len; ii++){
                                    if(yylval.str[ii] == '"'){
                                        yylval.str[ii] = '\0';
                                        break;
                                    }
                                }
                                struct literal lit = {.type = 1};
                                strcpy(lit.value.s_val, yylval.str);
                                add_literal(lit);
                                return STRING;
                            }

\'[^\'\n]*\'                {
                                yylval.str=strdup(yytext);
                                strcpy(yylval.str, yylval.str + 1);
                                int ii, len = strlen(yylval.str);
                                for(ii = 0; ii < len; ii++){
                                    if(yylval.str[ii] == '\''){
                                        yylval.str[ii] = '\0';
                                        break;
                                    }
                                }
                                struct literal lit = {.type = 1};
                                strcpy(lit.value.s_val, yylval.str);
                                add_literal(lit);
                                return STRING;
                            }

[a-zA-Z][a-zA-Z0-9_]*       yylval.str=strdup(yytext); return IDENTIFIER;
#[^\n]+                     /*  Just consume    */
"   "                       return INDENT;
\n                          return NEWLINE;
" "                         /*return WHITESPACE;*/
.                           ;

%%



#define check_dedent()  if(dedent_count){dedent_count--;return DEDENT;}
#define check_cached()     if(cached_token){int ret = cached_token;         \
                                cached_token = 0;return ret;}
#define count_indent_get_token() while(token == INDENT){current_indent++;   \
                                                        token = yylex();}
/*
 *  Take what we get from yylex and return the proper token based on indention
 *  level.  Adds tokens like dedent and ensures that we never indent more than
 *  once
 */
int _get_next_token()
{
    int current_indent = 0;
    //check if we are still dedenting
    check_dedent();
    //check if we had a token stored away because of indent or dedent
    check_cached();


    if(token == NEWLINE){
        first_in_line = 1;
    }
    //continue getting tokens until we don't get whitespace

    token = yylex();

    //if we are the first in line or not it doesn't matter.  Preserve our state
    //and return the newline token
    if(token == NEWLINE){
        return token;
    }

    //if we get a newline character, we start a new line.  Indents are only
    //significant at the beginning of a line
    if(! first_in_line){
        //if this isn't the first entry in the line, we will just treat an
        //indent like a space
        return token != INDENT ? token : WHITESPACE;
    }
    first_in_line = 0;

    //store our current indention level and then store the token we get.
    count_indent_get_token();

    //we have more indents than before, we are in a new block.  We need to
    //return the indent token and then store the other token we have
    if(current_indent > global_indent_level){
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

/*
    This function is sort of a hack, we use this as a wrapper for the yylex
    function.  Because of the INDENT/DEDENT tokens that we need for a python
    grammar, we need to return different tokens based on the number of indents
    we see at any given point in comparison to the number of indents we saw
    before.

    There are also some additional work arounds in this function.
        If we see an if token, we mark a global flag.
            We do this because if we see another if stmt before seeing an else
            we run into issues.
            THe issue is sort of self inflicted because when we enter a new
            logical block (such as for a function or an ifstmt, wherever the
            indention is more than it was before) we push the number of stmts
            we had for that block onto our stack.

            The Parser knows how many tokens to pop off of the stack based on
            this num stmt global we have.  If we see an if stmt after another
            if, before an else, we have to make sure to pop off the correct
            number of things from the stack before entering a new block and
            pushing out current number of stmts onto the stack

        If we see an else stmt we need to respond and update the flag based on
        the ifstmt

        If we enter a function we need to also push the number of stmts for the
        previous block onto the stack
 */
int current_indent_level = 0;

#define MAX_INDENT_LEVEL 10
int waiting_on_else [MAX_INDENT_LEVEL];

//basically we zero out all of the counter structures until we find the level
//where this if stmt was defined.  n is the new indent level
void pop_if(int n){
    for( ; n < current_indent_level; current_indent_level--){
        waiting_on_else[current_indent_level] = 0;
    }
    waiting_on_else[current_indent_level] = 1;
}

int disregard = 0;

int disregard_else(){
    token = _get_next_token();
    if(token != COLON){
        return token;
    }
    token = _get_next_token();
    if(token != NEWLINE){
        return token;
    }
    token = _get_next_token();
    if(token != INDENT){
        return token;
    }
    do {
        token = _get_next_token();
    } while(current_indent_level < global_indent_level);
    if(token != DEDENT){
        return token;
    }
    token = _get_next_token();

    disregard = 0;
    return token;
}

//if we recieve an else token for a indent level that is much less than the
//indent level we are currently considering, we wait to mark the w.o.e. for all
//of those higher indents to 0 because we will no longer be looking for a else
//token for them.
void do_woe(int n){
    assert(n <= current_indent_level);
    for( ; n < current_indent_level; current_indent_level--){
        waiting_on_else[current_indent_level] = 0;
    }
    waiting_on_else[current_indent_level] = 0;
}


int get_next_token()
{
    int token;

    //we have not entered a new block because we needed to return the if token
    //to the parser so it could pop the correct number of tokens from the stack
    if(delayed_block){
        enter_block();
        delayed_block = 0;
    }

loop_begin:

    do{
        if(disregard){
            token = disregard_else();
        }else{
            token = _get_next_token();
        }

        if(token == IF){
            if(waiting_on_else[current_indent_level]){
                if(current_indent_level == global_indent_level){
                    //we just mark for delay because we will still be waiting on
                    //else.  We return the if so that the parser knows to pop
                    //elements off of the stack
                    delayed_block = 1;

                //this means that we have incremented more since the last if
                //statement
                }else if(global_indent_level > current_indent_level){
                    //we are going to enter a new block
                    current_indent_level = global_indent_level;
                    waiting_on_else[current_indent_level] = 1;
                    enter_block();

                }else/*lower indent level*/{
                    pop_if(global_indent_level);
                    delayed_block = 1;
                }
            } else /*this should only happen for first if for indent level*/{
                current_indent_level = global_indent_level;
                waiting_on_else[current_indent_level] = 1;
                enter_block();
            }

        }else if(token == ELSE){
            if(waiting_on_else[global_indent_level]){
                do_woe(global_indent_level);
                enter_block();
            }else/*We were not expecting an else here*/{
                orphan_else++;
                current_indent_level = global_indent_level;
                disregard = 1;
                //gotos are fucking useful, fuck dikstra
                goto loop_begin;
            }

        }else if( token == DEF){
            enter_block();
        }

    } while(token == WHITESPACE);

    return token;
}

void set_input_file(char * str)
{
    yyin=fopen(str, "r");
}
