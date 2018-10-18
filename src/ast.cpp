/*
 *  This module holds the functions to create an AST, mainly the functions that
 *  create various nodes for that AST.
 *
 */

#include "ast.h"
#include "node_util.h"

/*  Creates a program node, a program node is the root node of a program.  It
 *  can contain any number of chilren.
 */
node_ptr create_program_node(int num_children, node_array children)
{
    create_node(ret);
    ret->type = PROGRAM_NODE;
    assign_children();
    return ret;
}

/*  A number node represents a single integer number
 */
node_ptr create_number_node (long value)
{
    create_node(ret);
    ret->type = NUMBER_NODE;
    ret->value.n_value = value;
    return ret;
}

/*  An expr node is anything in which an operator is involved in: x < y, 5 + 7,
 *  etc>.
 *
 *  The operator is expected to be the same enum type found representing the
 *  tokens in both  the lexer and parser files.  It can be included with
 *  parser.tab.y
 */
node_ptr create_expr_node(int i_operator, int num_children, node_array children)
{
    create_node(ret);
    ret->type = EXPR_NODE;
    ret->i_operator = i_operator;
    assign_children();

    return ret;
}

/*  String type
 */
node_ptr create_string_node(char * value)
{
    create_node(ret);
    ret->type = STRING_NODE;
    copy_sval(value);
    return ret;
}

/*  some variable
 */
node_ptr create_identifier_node(char * name)
{
    create_node(ret);
    ret->type = IDENTIFIER_NODE;
    copy_sval(name);
    return ret;
}

/* expects an identifier type on the left and either an expression, number, or
 * string on the right.
 */
node_ptr create_assignment_node(node_ptr ident, node_ptr value)
{
    create_node(ret);
    ret->type = ASSIGNMENT_NODE;
    ret->num_children = 2;
    ret->children = (ParseTreeNode **)malloc(sizeof(node_ptr) * 2);
    ret->children[0] = ident;
    ret->children[1] = value;

    return ret;
}

/*  print node type, has any number of children.
 */
node_ptr create_print_node(int num_children, node_array children)
{
    create_node(ret);
    ret->type = PRINT_STMT_NODE;
    assign_children();
    return ret;
}

/*  A node to represent a grouping of statments found inside both if/else and
 *  function definitions.  It can contain any number of any kind of stmt
 */
node_ptr create_block_stmt_node(int num_stmts, node_array stmts)
{
    create_node(ret);
    ret->type = BLOCK_STMT_NODE;
    ret->num_children = num_stmts;
    ret->children = stmts;
    return ret;
}

/*  Node for a function definition.  the parameters represent the name of the
 *  function and a block stmt which contains all of the stmts to execute this
 *  node.
 */
node_ptr create_func_def_node(char* name, node_ptr block_stmt)
{
    create_node(ret);
    ret->type = FUNC_DEF_NODE;

    create_children(1);
    ret->children[0] = block_stmt;
    copy_sval(name);

    return ret;
}

/*  Node represinting just a if statment with no else block.  The condition can
 *  be an expression and the block is expected to be a block stmt
 */
node_ptr create_branch_no_else_node(node_ptr condition, node_ptr block)
{
    create_node(ret);
    ret->type = BRANCH_STMT_NO_ELSE_NODE;

    create_children(2);
    ret->children[0] = block;
    ret->children[1] = condition;

    return ret;
}

/*  branch node that contains an else block.  Condition is again an expr (which
 *  if evaluated to non zero is considered true) and then both the if and else
 *  block stmts should be block stmt nodes
 */
node_ptr create_branch_with_else_node(node_ptr condition, node_ptr if_block,
                                      node_ptr else_block)
{
    create_node(ret);
    ret->type = BRANCH_STMT_ELSE_NODE;
    create_children(3);
    ret->children[0] = if_block;
    ret->children[1] = else_block;
    ret->children[2] = condition;
    return ret;
}

node_ptr create_func_call_node(char * name)
{
    create_node(ret);
    ret->type = FUNC_CALL_NODE;
    copy_sval(name);
    return ret;
}

node_ptr create_return_stmt(node_ptr ret_val)
{
    create_node(ret);
    ret->type = RETURN_STMT_NODE;
    create_children(1);
    ret->children[0] = ret_val;
    return ret;
}

/*  Destroys an entire tree by being given the root node.  WIll deallocate all
 *  memory
 */
void destroy_tree(ParseTreeNode * root)
{
    for(int ii = 0; ii < root->num_children; ii++){
        destroy_tree(root->children[ii]);
        root->children[ii] = 0;
    }
    if(root->num_children > 0)
        free(root->children);

    if(root->type == STRING_NODE || root->type == IDENTIFIER_NODE ||
       root->type == FUNC_DEF_NODE)
        free(root->value.s_value);

    delete root;
}

/*  Shorthand function that parses the input file (or stdin if the input file
 *  was not declared) and then returns the root of a constructed AST
 */
node_ptr get_ast()
{
    yyparse();
    create_program();
    return pop_node_from_stack();
}
