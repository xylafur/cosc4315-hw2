#ifndef NODE_UTIL
#define NODE_UTIL

#include "ast.h"

#define STACK_SIZE 1024

/*
 * I like macros..
 *
 * mostly just for convienience, pop or push particular number of elements 
 * off or onto the stack for particular situations.  Create new nodes, allocate
 * memory etc
 */
//this one apply's negation to the operand if the mod val is switched because
//there was a unary -
#define apply_negation_push() if(mod == -1){                                \
                                node_ptr neg_one = create_number_node(-1);  \
                                node_array children = alloc_children(2);    \
                                children[0] = node;                         \
                                children[1] = neg_one;                      \
                                node_ptr expr = create_expr_node(STAR, 2,   \
                                                                 children); \
                                push_node_to_stack(expr);                   \
                            } else{                                         \
                                push_node_to_stack(node);                   \
                            }

//get the right and left hand sides of an expression from the stack
#define get_sides()     node_ptr rhs = pop_node_from_stack();       \
                        node_ptr lhs = pop_node_from_stack();       \
                        node_array children = alloc_children(2);    \
                        children[0] = lhs;                          \
                        children[1] = rhs

//gets the sides of an expression and pushes the correct operator node onto the
//stack
#define push_operator(oper) get_sides();                                \
                            node_ptr node = create_expr_node(oper, 2,   \
                                                             children); \
                            push_node_to_stack(node)
 
void print_node_type(node_ptr node);
void print_tree(node_ptr root, int indent_level);

void push_node_to_stack(node_ptr node);
node_ptr pop_node_from_stack();

void print_stack();
void create_program();

#endif
