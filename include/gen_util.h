#ifndef GEN_UTIL
#define GEN_UTIL

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct literal {
    int type;
    union value{
        long n_val;
        char s_val [1024];
    } value;
} literal;

struct megastring {
    char * str;
    unsigned int num_strings;
    unsigned int where[1024];
} megastring;

#ifdef __cplusplus
extern "C" {
#endif

void add_literal(struct literal lit);
void print_literals();
struct literal * get_literals();
unsigned int get_num_literals();
void print_literal(struct literal lit);

struct megastring get_megastring();
void print_megastring();
int get_string_index(char * str);

int * get_int_literals();
unsigned int get_int_literals_length();
void print_int_literals();
int get_int_index(long val);

#ifdef __cplusplus
}
#endif




#endif
