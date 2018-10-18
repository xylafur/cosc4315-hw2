#ifndef GEN_UTIL
#define GEN_UTIL

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LITERAL_LIST_SIZE 256
#define MAX_NUM_LITERALS 1024
#define MAX_STRING_LITERAL_LENGTH 1024

//generic literal type.  Represents both string and number literals
//I really should just add the literal to the correct structure array based on
//its type rather than iterating through it again.
struct literal {
    int type;
    union value{
        long n_val;
        char s_val [MAX_STRING_LITERAL_LENGTH];
    } value;
} literal;

//structure to represent the concatination of many strings
struct megastring {
    char * str;
    unsigned int num_strings;
    unsigned int where[MAX_NUM_LITERALS];
} megastring;

/*  I wrote the gen util file in C, just because I'm much more comfortable with
 *  than c++ and there was no need for c++ in this file.
 *
 *  __cplusplus is defined if we are being compiled with a c++ file and a c++
 *  compiler.  The extern "C" { stmt is needed because c++ does some funky
 *  stuff with the names of functions because of operator overloading so this
 *  is needed to tell the c++ compiler to not mess with out identifier names
 */
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
