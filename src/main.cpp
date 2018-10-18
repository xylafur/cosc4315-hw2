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

    print_tree(ast_root, 0);
}
