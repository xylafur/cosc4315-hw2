#include "ast.h"
#include "node_util.h"

node_ptr create_program_node(int num_children, node_array children)
{
    create_node(ret);
    ret->type = PROGRAM_NODE;
    assign_children();
    return ret;
}

node_ptr create_number_node (long value)
{
    create_node(ret);
    ret->type = NUMBER_NODE;
    ret->value.n_value = value;
    return ret;
}

node_ptr create_expr_node(int i_operator, int num_children, node_array children)
{
    create_node(ret);
    ret->type = EXPR_NODE;
    ret->i_operator = i_operator;
    assign_children();

    return ret;
}

node_ptr create_string_node(char * value)
{
    create_node(ret);
    ret->type = STRING_NODE;
    copy_sval(value);
    return ret;
}

node_ptr create_identifier_node(char * name)
{
    create_node(ret);
    ret->type = IDENTIFIER_NODE;
    copy_sval(name);
    return ret;
}

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

node_ptr create_print_node(int num_children, node_array children)
{
    create_node(ret);
    ret->type = PRINT_STMT_NODE;
    assign_children();
    return ret;
}

node_ptr create_block_stmt_node(int num_stmts, node_array stmts)
{
    create_node(ret);
    ret->type = BLOCK_STMT_NODE;
    ret->num_children = num_stmts;
    ret->children = stmts;
    return ret;
}

node_ptr create_func_def_node(char* name, node_ptr block_stmt)
{
    create_node(ret);
    ret->type = FUNC_DEF_NODE;

    create_children(1);
    ret->children[0] = block_stmt;
    copy_sval(name);

    return ret;
}

node_ptr create_branch_no_else_node(node_ptr condition, node_ptr block)
{
    create_node(ret);
    ret->type = BRANCH_STMT_NO_ELSE_NODE;

    create_children(2);
    ret->children[0] = block;
    ret->children[1] = condition;

    return ret;
}

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

#ifdef MAKE_MAIN
int main()
{
    node_array children;

    node_ptr x = create_identifier_node("x");
    node_ptr five = create_number_node(5);
    node_ptr six = create_number_node(6);

    children = alloc_children(2);
    children[0] = five;
    children[1] = six;
    node_ptr five_plus_six = create_expr_node('+', 2, children);


    node_ptr x_eq_5_plus_6 = create_assignment_node(x, five_plus_six);

    node_ptr True = create_number_node(1);

    children = alloc_children(1);
    children[0] = x_eq_5_plus_6;
    node_ptr block = create_block_stmt_node(1, children);

    node_ptr branch = create_branch_no_else_node(True, block);

    children = alloc_children(1);
    children[0] = branch;   
    node_ptr program = create_program_node(1, children);

    print_tree(program, 0);

    destroy_tree(program);
}

#endif
