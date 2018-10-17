#include <stdio.h>

#include "parser.tab.h"
#include "ast.h"
#include "node_util.h"

extern FILE *yyin;

int main (int argc, char * argv[])
{

    if (argc == 2){
        yyin=fopen(argv[1], "r");
    }

    yyparse();
    create_program();
    print_stack();

}
