#include "gen_util.h"

#define MAX_LITERAL_LIST_SIZE 256
unsigned int literal_list_size = 0;
struct literal literal_list [MAX_LITERAL_LIST_SIZE];

unsigned int int_literals_size;
int int_literals [1024];

void add_literal(struct literal lit)
{
    int ii, found = 0;
    for(ii = 0; ii < literal_list_size; ii++){
        if(literal_list[ii].type == lit.type){
            if(lit.type == 1){
                if(!strcmp(lit.value.s_val, literal_list[ii].value.s_val)){
                    found = 1;
                    break;
                }
            }else{
                if(lit.value.n_val == literal_list[ii].value.n_val){
                    found = 1;
                    break;
                }
            }
        }
    }
    if(!found){
        literal_list[literal_list_size++] = lit;
    }
}

void print_literal(struct literal lit)
{
    if(lit.type == 1){
        printf("%s\n", lit.value.s_val);
    }else{
        printf("%li\n", lit.value.n_val);
    }
}

void print_literals()
{
    int ii;
    for(ii = 0; ii < literal_list_size; ii++){
        print_literal(literal_list[ii]);
    }
}

struct literal * get_literals ()
{
    return literal_list;
}


unsigned int get_num_literals()
{
    return literal_list_size;
}

struct megastring mega;

struct megastring get_megastring()
{
    unsigned int tot_len = 0, index = 0, cur_len=0, num_strings = 0;
    int ii;
    unsigned int num_literals = get_num_literals();
    struct literal * literals = get_literals();

    for(ii = 0; ii < num_literals; ii++){
        if(literals[ii].type == 1){
            tot_len += 1 + strlen(literals[ii].value.s_val);
            num_strings++;
        }
    }
    mega.str = (char*)malloc(tot_len);
    mega.num_strings = num_strings;
    for(ii = 0; ii < num_literals; ii++){
        if(literals[ii].type == 1){
            strcpy(mega.str + cur_len, literals[ii].value.s_val);
            mega.where[index++] = cur_len;
            cur_len += strlen(literals[ii].value.s_val) + 1;
        }
    }
    return mega;
}

void print_megastring()
{
    int ii;
    for(ii = 0; ii < mega.num_strings; ii++){
        printf("%s\n", mega.str + mega.where[ii]);
    }
}

int get_string_index(char * str)
{
    int ii;
    for(ii = 0 ; ii < mega.num_strings; ii++){
        if(!strcmp(str, mega.str + mega.where[ii])){
             return mega.where[ii];
        }
    }
    return -1;
}



int * get_int_literals()
{
    int ii;
    unsigned int num_literals = get_num_literals();
    struct literal * literals = get_literals();

    for(ii = 0; ii < num_literals; ii++){
        if(literals[ii].type == 0){
            int_literals[int_literals_size++] = literals[ii].value.n_val;
        }
    }
    return int_literals;
}

unsigned int get_int_literals_length()
{
    return int_literals_size;
}

int get_int_index(long val)
{
    int ii;
    for(ii = 0; ii < int_literals_size; ii++){
        if(int_literals[ii] == val)
            return ii;
    }
    return -1;
}

void print_int_literals()
{
    int ii;
    for(ii = 0; ii < int_literals_size; ii++){
        printf("%d\n", int_literals[ii]);
    }
}
