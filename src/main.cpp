#include <stdio.h>
#include "parser.tab.h"
#include "ast.h"
#include "node_util.h"
#include "gen_util.h"

#include "if_else_detector.h"
#include "mutation_detector.h"
#include "recursion_detector.h"

#include <vector>
#include <string>
#include <iostream>

extern int orphan_else;
extern FILE *yyin;

int main (int argc, char * argv[])
{
    int do_tree = 0;
    /*  If we are given a file as a input we will parse that file, otherwise
     *  just grab lines from stdinput until the user types ctrl+^D
     */
    if (argc == 2){
        yyin=fopen(argv[1], "r");
    } else if (argc == 3) {
        do_tree = 1;
        yyin=fopen(argv[2], "r");
        
    } else if (argc > 3) {
        printf("Expects one argument (input file) at max!\n");
        return 1;
    }

    /*  Get the root of the abstract syntax tree
     */
    node_ptr ast_root = get_ast();
    /* print the entire tree.  We would pass the tree into the conversion
     * function and then it would be executed by the VM, if it were completed
     */
    if(do_tree){
        print_tree(ast_root, 0);
    }



    std::vector <int> if_elses = detect_if_else(ast_root);
    std::vector <std::string> mutations = find_mutations(ast_root);

    if(orphan_else > 0){
        printf("ERROR: Found %d else blocks without an if\n", orphan_else);
    }

    printf("\nIf/else nested levels: ");
    if(if_elses.size() > 0){
        for(int ii : if_elses){
            printf("%d, ", ii);
        }
        printf("\n");
    }else{
        printf("None.\n");
    }

    if(mutations.size() > 0){
        printf("\nMutated variable: ");
        for(std::string ss : mutations) {
            std::cout << ss << " level, ";
        }
        puts("\n\n");
    }

    std::vector<ParseTreeNode*> func_calls = find_global_func_calls(ast_root);
    std::vector<ParseTreeNode*> func_defs = find_recursive_functions(ast_root);

    for(int ii= 0; ii < func_defs.size(); ii++){
        for(int jj = 0; jj < func_calls.size(); jj++){
            if(strcmp(func_defs[ii]->value.s_value,
                      func_calls[jj]->value.s_value) == 0){
                //printf("Found call of func %s\n", func_calls[jj]->value.s_value);
                std::cout << recursive_function_terminates(
                        func_defs[ii], func_calls[jj]) << std::endl;
            }
        }
        //print_tree(func_defs[ii], 0);
        //recursive_function_terminates(
        
    }

    printf("\n\n");
    //don't forget to deallocate
    destroy_tree(ast_root);
}
