#include "node_util.h"

void print_tree(ParseTreeNode * root, int indent_level)
{
    for(int ii = 0; ii < indent_level; ii++)
        printf("    ");

    switch(root->type){
        case PROGRAM:
            printf("PROGRAM\n");
            break;
        case MULT_STMTS:
            printf("MULT_STMTS\n");
            break;
        case FUNC_DEF:
            printf("FUNC_DEF: %s\n", root->value.s_value);
            break;
        case BLOCK_STMT:
            printf("BLOCK STMT\n");
            break;
        case BRANCH_STMT_NO_ELSE:
            printf("BRANCH_STMT_NO_ELSE\n");
            break;
        case BRANCH_STMT_ELSE:
            printf("BRANCH_STMT_ELSE\n");
            break;
        case PRINT_STMT:
            printf("PRINT_STMT\n");
            break;
        case ASSIGN:
            printf("ASSIGN\n");
            break;
        case FUNC_CALL:
            printf("FUNC_CALL\n");
            break;
        case RETURN_STMT:
            printf("RETURN_STMT\n");
            break;
        case EXPR:
            printf("EXPR\n");
            break;
        case STRING:
            printf("STRING\n");
            break;
        case NUMBER:
            printf("NUMBER: %d\n", root->value.n_value);
            break;
        case IDENTIFIER:
            printf("IDENTIFIER: %s\n", root->value.s_value);
            break;
        default:
            break;
    }
    for(int ii = 0; ii < root->num_children; ii++){
        print_tree(root->children[ii], indent_level + 1);
    }
}
