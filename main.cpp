#include <stdio.h>

#include "tokens.h"
#include "lexer.h"

int main (int argc, char * argv[])
{
    if (argc != 2){
        printf("Need input file\n");
        return 1;
    }

    set_input_file(argv[1]);
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
