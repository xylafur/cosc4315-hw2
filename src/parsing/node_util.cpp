/*  Helper functions of the AST module.. it should probably be merged with the
 *  AST module because they have many of the same ufncitonalities.  The print
 *  stmts are just very long because of the huge switch so I wanted to get it
 *  out of the ast file
 */
#include "node_util.h"

void get_oper(node_ptr node, char oper [4])
{
    switch(node->i_operator){
    case PLUS:
        oper[0] = '+';
        oper[1] = '\0';
        break;
    case MINUS:
        oper[0] = '-';
        oper[1] = '\0';
        break;
    case STAR:
        oper[0] = '*';
        oper[1] = '\0';
        break;
    case SLASH:
        oper[0] = '/';
        oper[1] = '\0';
        break;
    case EQUALSEQUALS:
        oper[0] = '=';
        oper[1] = '=';
        oper[2] = '\0';
        break;
    case AND:
        oper[0] = 'a';
        oper[1] = 'n';
        oper[2] = 'd';
        oper[3] = '\0';
        break;
    case OR:
        oper[0] = 'o';
        oper[1] = 'r';
        oper[2] = '\0';
        break;
    case GREATEREQUALS:
        oper[0] = '>';
        oper[1] = '=';
        oper[2] = '\0';
        break;
    case LESSEQUALS:
        oper[0] = '<';
        oper[1] = '=';
        oper[2] = '\0';
        break;
    case GREATER:
        oper[0] = '>';
        oper[1] = '\0';
        break;
    case LESS:
        oper[0] = '<';
        oper[1] = '\0';
        break;
    }
}

void print_node_type(node_ptr node)
{
    switch(node->type){
        case PROGRAM_NODE:
            printf("PROGRAM\n");
            break;
        case MULT_STMTS_NODE:
            printf("MULT_STMTS\n");
            break;
        case FUNC_DEF_NODE:
            printf("FUNC_DEF: %s\n", node->value.s_value);
            break;
        case BLOCK_STMT_NODE:
            printf("BLOCK STMT w/ %d children\n", node->num_children);
            break;
        case BRANCH_STMT_NO_ELSE_NODE:
            printf("BRANCH_STMT_NO_ELSE\n");
            break;
        case BRANCH_STMT_ELSE_NODE:
            printf("BRANCH_STMT_ELSE\n");
            break;
        case PRINT_STMT_NODE:
            printf("PRINT_STMT\n");
            break;
        case ASSIGNMENT_NODE:
            printf("ASSIGN\n");
            break;
        case FUNC_CALL_NODE:
            printf("FUNC_CALL\n");
            break;
        case RETURN_STMT_NODE:
            printf("RETURN_STMT\n");
            break;
        case EXPR_NODE:
            char oper [4];
            get_oper(node, oper);
            printf("EXPR: %s\n", oper);
            break;
        case STRING_NODE:
            printf("STRING: %s\n", node->value.s_value);
            break;
        case NUMBER_NODE:
            printf("NUMBER: %d\n", node->value.n_value);
            break;
        case IDENTIFIER_NODE:
            printf("IDENTIFIER: %s\n", node->value.s_value);
            break;
        default:
            break;
    }
}

void print_tree(ParseTreeNode * root, int indent_level)
{
    for(int ii = 0; ii < indent_level; ii++)
        printf("    ");
    print_node_type(root);

    for(int ii = 0; ii < root->num_children; ii++){
        print_tree(root->children[ii], indent_level + 1);
    }
}

node_ptr node_stack [STACK_SIZE];
unsigned int stack_pos = 0;

void push_node_to_stack(node_ptr node)
{
    node_stack[stack_pos++] = node;
}

node_ptr pop_node_from_stack()
{
    if(stack_pos == 0){
        printf("THE STACK IS EMPTY YOU FUCKING IDIOT\n");
    }
    return node_stack[--stack_pos];
}


void print_stack()
{
    printf("There are %d nodes in the stack!\n", stack_pos);
    while(stack_pos){
        node_ptr node = pop_node_from_stack();
        print_tree(node, 0);
    }
}

void create_program()
{
    node_array children = alloc_children(stack_pos);
    int len = stack_pos;
    for(int ii = 0; ii < len; ii++){
        children[len - ii - 1] = pop_node_from_stack();
    }
    node_ptr program = create_program_node(len, children);
    push_node_to_stack(program);
}
