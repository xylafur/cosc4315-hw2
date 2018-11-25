
#ifndef PARSER_TREE_NODE_H
#define PARSER_TREE_NODE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "parser.tab.h"

typedef enum parse_tree_type_s {
    PROGRAM_NODE,

    BLOCK_STMT_NODE,
    MULT_STMTS_NODE, // not used

    FUNC_DEF_NODE,
    BRANCH_STMT_NO_ELSE_NODE,
    BRANCH_STMT_ELSE_NODE,

    PRINT_STMT_NODE,
    ASSIGNMENT_NODE,
    FUNC_CALL_NODE,
    RETURN_STMT_NODE,

    EXPR_NODE,
    STRING_NODE,
    NUMBER_NODE,

    IDENTIFIER_NODE
} parse_tree_type_t;

#define node_ptr struct ParseTreeNode *
#define node_array struct ParseTreeNode **
#define create_node(name) node_ptr name = new ParseTreeNode
#define delete_node(name) delete name

#define alloc_children(num) (node_array)malloc(sizeof(node_ptr) * num)
#define create_children(num) ret->num_children = num;       \
    ret->children = (node_array)malloc(sizeof(node_ptr) * num)
#define assign_children() ret->num_children = num_children; \
                          ret->children = children

#define copy_sval(name) size_t length = strlen(name); \
                        ret->value.s_value = (char*)malloc(length);\
                        strcpy(ret->value.s_value, name)

struct ParseTreeNode {
    parse_tree_type_t type;
    unsigned int num_children = 0;
    unsigned int curr_child = 0;
    ParseTreeNode ** children = 0;

    int i_operator = 0;

    union value {
        char * s_value;
        long n_value;
    } value;
};

node_ptr get_ast();

node_ptr create_program_node(int num_children, node_array children);

node_ptr create_number_node (long value);

node_ptr create_expr_node(int i_operator, int num_children,
                          node_array children);

node_ptr create_string_node(char * value);

node_ptr create_identifier_node(char * name);

node_ptr create_assignment_node(node_ptr ident, node_ptr value);

node_ptr create_print_node(int num_children, node_array children);

node_ptr create_block_stmt_node(int num_stmts, node_array stmts);

node_ptr create_func_def_node(char* name, node_ptr block_stmt);

node_ptr create_branch_no_else_node(node_ptr condition, node_ptr block);

node_ptr create_branch_with_else_node(node_ptr condition, node_ptr if_block,
                                      node_ptr else_block);

node_ptr create_func_call_node(char * name);

node_ptr create_return_stmt(node_ptr ret_val);

void destroy_tree(node_ptr root);
#endif

