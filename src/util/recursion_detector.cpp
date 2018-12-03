
#include "recursion_detector.h"
using namespace std;

bool contains_func_call_of(ParseTreeNode *func_node, const char *func_name) {
    /*
     * To determine if a FUNC_DEF_NODE node is recursive, call like this:
     * >> contains_func_call_of(func_def, func_def->value.s_value);
     *
     * */

    queue<ParseTreeNode *> node_queue;
    node_queue.push(func_node);
    ParseTreeNode *temp;

    // use breadth first search to find a recursive call on self
    while (!node_queue.empty()) {
        temp = node_queue.front(); node_queue.pop();
        if (temp->type == FUNC_CALL_NODE
             && strcmp(temp->value.s_value, func_name) == 0)
        {
            return true;
        }

        for (int i = 0; i < temp->num_children; i++) {
            node_queue.push(temp->children[i]);
        }
    }

    return false;
}

bool is_recursive_function(ParseTreeNode *func_def) {
    assert(func_def && func_def->type == FUNC_DEF_NODE);
    return contains_func_call_of(func_def, func_def->value.s_value);
}

bool is_base_return_stmt(ParseTreeNode *ret_stmt, const char *func_name) {
    assert(ret_stmt && ret_stmt->type == RETURN_STMT_NODE);
    return !contains_func_call_of(ret_stmt, func_name);
}

vector<ParseTreeNode*>
find_return_statements(ParseTreeNode *func_def) {
    /* This function finds all the return nodes inside a func_def */
    vector<ParseTreeNode*> return_stmts;

    queue<ParseTreeNode *> node_queue;
    node_queue.push(func_def);

    ParseTreeNode *temp;

    while (!node_queue.empty()) {
        temp = node_queue.front(); node_queue.pop();

        if (temp->type == RETURN_STMT_NODE) {
            return_stmts.push_back(temp);
            continue;
        }

        for (int i = 0; i < temp->num_children; i++) {
            node_queue.push(temp->children[i]);
        }
    }

    return return_stmts;
}

bool recursive_function_terminates
(ParseTreeNode * rec_func, ParseTreeNode * call) {

    assert(rec_func->type == FUNC_DEF_NODE);
    assert(call->type == FUNC_CALL_NODE);

    const char * func_name = rec_func->value.s_value;

    ParseTreeNode * func_block = rec_func->children[0];

    long * calling_values = new long[call->num_children];
    printf("%s(", func_name);
    for (int i = 0; i < call->num_children; i++) {
        printf("%d,", evaluate_expr(call->children[i], 0));
    }
    printf(") :");
    delete[] calling_values;

    if (rec_func->num_parameters == 0) return false;
    else if (rec_func->num_parameters > 1) goto flip_a_coin;

    if (func_block->children[0]->type == RETURN_STMT_NODE){
        // printf("first thing is a return\n");
        return is_base_return_stmt(rec_func->children[0], func_name);
    }

    if (func_block->children[0]->type == BRANCH_STMT_ELSE_NODE) {

        ParseTreeNode * if_else_block = func_block->children[0];

        ParseTreeNode * condition    = if_else_block->children[2];
        ParseTreeNode * _true_block  = if_else_block->children[0];
        ParseTreeNode * _false_block = if_else_block->children[1];

        ParseTreeNode *if_ret       = _true_block->children[0];
        ParseTreeNode *else_ret     = _false_block->children[0];

        assert(if_ret->type == RETURN_STMT_NODE);
        assert(else_ret->type == RETURN_STMT_NODE);

        bool recurse_if_false;
        if (is_base_return_stmt(if_ret, func_name)
                && contains_func_call_of(else_ret, func_name))
        {
            recurse_if_false = true;
        } else if (is_base_return_stmt(else_ret, func_name)
                && contains_func_call_of(if_ret, func_name))
        {
            recurse_if_false = false;
        } else {
            goto flip_a_coin;
        }

        // find shallowest func call node
        ParseTreeNode *func_call;
        if (recurse_if_false)
            func_call = find_shallowest_func_call(else_ret, func_name);
        else
            func_call = find_shallowest_func_call(if_ret, func_name);

        long calling_value = evaluate_expr(call->children[0], 0);
        long before  = condition_evaluator(condition, calling_value);
        long augment = evaluate_expr(func_call->children[0], calling_value);
        long after   = condition_evaluator(condition, augment);


        #define ABS(a) ((a < 0) ? (-a) : (a))

        if (recurse_if_false) {

            if (condition->i_operator == EQUALSEQUALS) {
                if (before == 0 || after == 0) return true;
            }
            if (condition->i_operator == GREATEREQUALS) {
                if (before >= 0 || after >= 0) return true;
            }
            if (condition->i_operator == LESSEQUALS) {
                if (before <= 0 || after <= 0) return true;
            }
            if (condition->i_operator == LESS) {
                if (before < 0 || after < 0) return true;
            }
            if (condition->i_operator == GREATER) {
                if (before > 0 || after > 0) return true;
            }

            return ABS(after) < ABS(before);

        } else { // recurse if true

            if (condition->i_operator == EQUALSEQUALS) {
                if (before != 0 || after != 0) return true;
            }
            if (condition->i_operator == GREATEREQUALS) {
                if (before < 0 || after < 0) return true;
            }
            if (condition->i_operator == LESSEQUALS) {
                if (before > 0 || after > 0) return true;
            }
            if (condition->i_operator == LESS) {
                if (before >= 0 || after >= 0) return true;
            }
            if (condition->i_operator == GREATER) {
                if (before <= 0 || after <= 0) return true;
            }

            return ABS(after) > ABS(before);

        }
    }

    flip_a_coin:
    //puts("...Flipping a coin to solve halting problem...");
    return (rand() & 1);
}

vector<ParseTreeNode*> find_global_func_calls
(ParseTreeNode* program_node, vector<ParseTreeNode*> &recursive_functions) {
    /*
     * finds all function calls in the program
     * assume that no nested function calls exist
     * */
    assert(program_node);
    assert(program_node->type==PROGRAM_NODE);

    vector<ParseTreeNode*> func_calls;
    queue<ParseTreeNode*> search_areas;

    ParseTreeNode *temp;

    for (int i = 0; i < program_node->num_children; i++) {
        temp = program_node->children[i];
        if (temp->type != FUNC_DEF_NODE) {
            search_areas.push(temp);
        }
    }

    while (!search_areas.empty()) {
        temp = search_areas.front(); search_areas.pop();

        if (temp->type == FUNC_CALL_NODE) {
            for (auto rec : recursive_functions) {
                if (strcmp(rec->value.s_value, temp->value.s_value)==0) {
                    func_calls.push_back(temp);
                    break;
                }
            }
            continue;
                // remove this if they have nested function calls : foo(foo(m))
        }

        for (int i = 0; i < temp->num_children; i++) {
            search_areas.push(temp->children[i]);
        }
    }

    return func_calls;
}

vector<ParseTreeNode*> find_recursive_functions(ParseTreeNode *start) {
    /* This function returns all func_def nodes which are recursive in nature
     * */

    // sanity checks
    assert(start);
    assert(start->type == PROGRAM_NODE);

    vector<ParseTreeNode*> functions;
    ParseTreeNode *temp;

    for (int i = 0; i < start->num_children; i++) {
        temp = start->children[i];
        if (temp->type == FUNC_DEF_NODE &&
            contains_func_call_of(temp, temp->value.s_value)) {
            functions.push_back(temp);
        }
    }

    return functions;
}

void clean_curr_childs(ParseTreeNode *node) {
    queue<ParseTreeNode*> node_queue;
    node_queue.push(node);

    ParseTreeNode *temp;
    while (!node_queue.empty()) {
        temp = node_queue.front(); node_queue.pop();
        temp->curr_child = 0;
        for (int i = 0; i < temp->num_children; i++) {
            node_queue.push(temp->children[i]);
        }
    }
}

ParseTreeNode * find_shallowest_func_call(ParseTreeNode *node,
                                          const char * func_name) {
    ParseTreeNode * ret = 0;

    queue<ParseTreeNode*> node_queue;
    node_queue.push(node);
    ParseTreeNode *temp;
    while (!node_queue.empty()) {
        temp = node_queue.front(); node_queue.pop();
        if (temp->type == FUNC_CALL_NODE
            && strcmp(temp->value.s_value, func_name) == 0)
        {
            ret = temp; break;
        }

        for (int i = 0; i < temp->num_children; i++) {
            node_queue.push(temp->children[i]);
        }
    }

    return ret;
}

long condition_evaluator(ParseTreeNode *condition, int val) {
    // assume following structure
    //        (CMP : <,>,>=,<=,==)
    //       /                    \
    //  (LHS)                      (RHS)
    //
    //
    //  want to change into
    //
    //                  (CMP : <,>,>=,<=,==)
    //                 /                    \
    //              (-)                      (0)
    //             /   \
    //        (LHS)     (RHS)
    //
    // that way condition becomes Truer if evaluation of
    // LHS - RHS is closer to zero

    ParseTreeNode *LHS = condition->children[0];
    ParseTreeNode *RHS = condition->children[1];

    ParseTreeNode SUB_NODE;
    SUB_NODE.type = EXPR_NODE;
    SUB_NODE.num_children = 2;
    SUB_NODE.children = new ParseTreeNode*[2];
    SUB_NODE.children[0] = LHS;
    SUB_NODE.children[1] = RHS;
    SUB_NODE.i_operator = MINUS;

    long eval = evaluate_expr(&SUB_NODE, val);
    SUB_NODE.children[0] = SUB_NODE.children[1] = 0;
    delete[] SUB_NODE.children;

    return eval;
}


long evaluate_expr(ParseTreeNode *expr, int val) {
    // assumptions:
    //  * expr is an arithmetic expression which is a child of a func call
    //  * val is the value to substitute

    stack<ParseTreeNode*> node_stack;
    node_stack.push(expr);

    stack<long> operands;
    ParseTreeNode *temp;

    while (!node_stack.empty()) {
        temp = node_stack.top(); 
        if (temp->curr_child == temp->num_children) {
            node_stack.pop();
            if (temp->type == IDENTIFIER_NODE) {
                operands.push(val);
            } else if (temp->type == NUMBER_NODE) {
                operands.push(temp->value.n_value);
            } else {
                int b = operands.top(); operands.pop();
                int a = operands.top(); operands.pop();

                switch (temp->i_operator) {
                    case PLUS  :  operands.push(a+b); break;
                    case MINUS :  operands.push(a-b); break;
                    case STAR  :  operands.push(a*b); break;
                    case SLASH :  operands.push(a/b); break;
                    default: puts("ERROR in is_increasing: unrecognized operator");
                             assert(0);
                }
            }
        } else {
            node_stack.push(temp->children[temp->curr_child++]);
        }
    }

    clean_curr_childs(expr);

    return operands.top();
}

