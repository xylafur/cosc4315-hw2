/*
 *  In this file I am including functions used by lex to get tokens and such.
 *  WE will need to use these in the parser, thus we need a reference to them
 *  in a header of somesort.
 *
 *  I know there is some way that flex and bison are to work together, I am not
 *  sure if this is the proper methodoligy.
 */

#ifndef LEXER_HEADER
    #define LEXER_HEADER

#include <stdio.h>
#include "tokens.h"

void print_type(int token);
void set_input_file(char * str);
int get_next_token(void);

#endif
