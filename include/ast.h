
#ifndef PARSER_TREE_NODE_H
#define PARSER_TREE_NODE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef enum parse_tree_type_s {
    PROGRAM,

    BLOCK_STMT,
    MULT_STMTS,

    FUNC_DEF,
    BRANCH_STMT_NO_ELSE,
    BRANCH_STMT_ELSE,

    PRINT_STMT,
    ASSIGN,
    FUNC_CALL,
    RETURN_STMT,

    EXPR,
    STRING,
    NUMBER,

    IDENTIFIER
} parse_tree_type_t;

#define node_ptr struct ParseTreeNode *
#define node_array struct ParseTreeNode **
#define create_node(name) node_ptr name = new ParseTreeNode
#define alloc_children(num) (node_array)malloc(sizeof(node_ptr) * num)

#define create_children(num) ret->num_children = num;       \
    ret->children = (node_array)malloc(sizeof(node_ptr) * num)

#define copy_sval(name) size_t length = strlen(name); \
                        ret->value.s_value = (char*)malloc(length);\
                        strcpy(ret->value.s_value, name)

struct ParseTreeNode {
    parse_tree_type_t type;
    unsigned int num_children = 0;
    ParseTreeNode ** children = 0;

    char c_operator = 0;
    int i_operator = 0;

    union value {
        char * s_value;
        long n_value;
    } value;
};

struct ParseTreeNode * create_number_node (long value);
struct ParseTreeNode * create_expr_node(char c_operator, int num_children,
                                        ParseTreeNode ** children);

void destroy_node (parse_tree_type_t type, ParseTreeNode * node);
#endif

