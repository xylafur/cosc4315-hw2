#include <stdio.h>
#include "parser.tab.h"
#include "ast.h"
#include "node_util.h"
#include "gen_util.h"
#include "variables.h"

extern FILE *yyin;

int main (int argc, char * argv[])
{

    /*  If we are given a file as a input we will parse that file, otherwise
     *  just grab lines from stdinput until the user types ctrl+^D
     */
    if (argc == 2){
        yyin=fopen(argv[1], "r");
    } else if (argc >2 ) {
        printf("Expects one argument (input file) at max!\n");
        return 1;
    }

    /*  Get the root of the abstract syntax tree
     */
    node_ptr ast_root = get_ast();

    /* print the entire tree.  We would pass the tree into the conversion
     * function and then it would be executed by the VM, if it were completed
     */
    print_tree(ast_root, 0);

    //don't forget to deallocate
    destroy_tree(ast_root);
}
