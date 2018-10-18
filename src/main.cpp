#include <stdio.h>
#include "parser.tab.h"
#include "ast.h"
#include "node_util.h"
#include "gen_util.h"

extern FILE *yyin;

int main (int argc, char * argv[])
{
    if (argc == 2){
        yyin=fopen(argv[1], "r");
    }

    node_ptr ast_root = get_ast();

    //print_tree(ast_root, 0);
    //print_literals();
    struct megastring mega = get_megastring();
    print_megastring();

    int where = get_string_index("Hello");
    int where2 = get_string_index("hello");

    printf("where: %d\n", where);
    printf("where2: %d\n", where2);

    int * int_literals = get_int_literals();
    print_int_literals();

    printf("5: %d\n", get_int_index(5));
    printf("6: %d\n", get_int_index(6));
}
