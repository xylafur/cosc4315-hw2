
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
             && strcmp(temp->value.s_value, func_name) == 0) {
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

bool recursive_function_terminates(ParseTreeNode * rec_func,
                                   ParseTreeNode * call){
    const char * func_name = rec_func->value.s_value;

    ParseTreeNode * block = rec_func->children[0];
    if(rec_func->num_children > 0){
        printf("num children > 0\n");
        if(rec_func->children[0]->type == RETURN_STMT_NODE){
            printf("first thing is a return\n");
            return is_base_return_stmt(rec_func->children[0], func_name);
        }

        else if(block->children[0]->type == BRANCH_STMT_ELSE_NODE){
            printf("branch stmt with else\n");

            ParseTreeNode * condition    = block->children[2];
            ParseTreeNode * _if     = block->children[0];
            ParseTreeNode * _else = block->children[1];

            ParseTreeNode * if_ret = _if->children[0]->children[0];
            ParseTreeNode * else_ret = _else->children[0]->children[0];
    
            //the if block is the recurisve call and the else block is the base
            //case
            if(is_recursive_function(if_ret) &&
               is_base_return_stmt(else_ret, func_name)){
                printf("if recursive, else base\n");

            }else if(is_recursive_function(else_ret) &&
                     is_base_return_stmt(if_ret, func_name)){
                printf("else recursive, if base\n");

            }else if(is_base_return_stmt(if_ret, func_name) &&
                     is_base_return_stmt(else_ret, func_name)){
                printf("else base, if base\n");

            }else if(is_recursive_function(if_ret) &&
                     is_recursive_function(else_ret)){
                printf("else recursive, if recursive\n");


            }else{
                //throw our hands up because we hard coded to hard
                printf("flip a coin!\n");
            }


        }
    }
    return false;
    
}

/*
bool recursive_function_terminates(ParseTreeNode *rec_func, vector<ParseTreeNode> calls) {
    assert(rec_func);
    assert(rec_func->type == FUNC_DEF_NODE);

    if (!contains_func_call_of(rec_func, rec_func->value.s_value)) {
        return false; // not a recursive function
    }

    vector<ParseTreeNode *> return_stmts = find_return_statements(rec_func);
    bool has_no_base_return = true;
    ParseTreeNode *base_return = 0, *recursive_call = 0;

    int counter=0, base_return_pos = 0, recursive_call_pos = 0;
    for (auto ret : return_stmts) {
        if (is_base_return_stmt(ret)) {
            has_no_base_return = false;
            base_return = ret;
            base_return_pos = counter;
            break;
        } else {
            recursive_call = ret;
            recursive_call_pos = counter;
        }
        counter++;
    }

    if (has_no_base_return) {
        return false;
    }

    // I STOPPED HERE

    ParseTreeNode *func_block = rec_func->children[0];
    assert(func_block->type == BLOCK_STMT_NODE); // or replace with return false

    ParseTreeNode *if_stmt_node = func_block->children[0];
        // assume that first 

    assert(if_stmt_node->type == BRANCH_STMT_ELSE_NODE
            || if_stmt_node->type == BRANCH_STMT_NO_ELSE_NODE);
            // or replace with return false

    ParseTreeNode *condition = if_stmt_node->children[if_stmt_node->num_children-1];
    assert(condition->type == EXPR_NODE);


    ParseTreeNode *recursive_call = 0;
    if (if_stmt_node->type == BRANCH_STMT_ELSE_NODE) {
        recursive_call = if_stmt_node->children[1].children[0];
    } else { // BRANCH_STMT_NO_ELSE_NODE
        // assume first statement after if statement is a return statement
        recursive_call = rec_func->children[1];
    }

    assert(recursive_call->type == RETURN_STMT_NODE); // or replace with return false

    bool base_case_if_true = contains_func_call_of(recursive_call, rec_func->value.s_value);

}
*/


vector<ParseTreeNode*> find_global_func_calls(ParseTreeNode* program_node) {
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
            func_calls.push_back(temp);
            continue; // remove this if they have nested function calls : foo(foo(m))
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

bool is_increasing(ParseTreeNode *expr, int val) {
    // assumptions:
    //  * expr is an arithmetic expression which is a child of a func call
    //  * val is the value to substitute

    stack<ParseTreeNode*> node_stack;
    node_stack.push(expr);

    stack<int> operands;
    ParseTreeNode *temp;

    while (!node_stack.empty()) {
        temp = node_stack.top(); 
        if (temp->curr_child == temp->num_children) {
            node_stack.pop();
            if (temp->type == IDENTIFIER_NODE) {
                operands.push(val);
            } else if (temp->type == NUMBER_NODE) {
                operands.push(temp->value.s_value);
            } else {
                int b = operands.top(); operands.pop();
                int a = operands.top(); operands.pop();
                if (temp->type == PLUS) {
                    operands.push(a+b);
                } else if (temp->type == MINUS) {
                    operands.push(a-b);
                } else if (temp->type == STAR) {
                    operands.push(a*b);
                } else if (temp->type == SLASH) {
                    operands.push(a/b);
                } else {
                    assert(0);
                }
            }
        } else {
            node_stack.push(temp->num_children[temp->curr_child++]);
        }
    }

    clean_curr_childs(expr);

    int eval = operands.top();
    return (eval == val) ? 0 : ((eval > val) ? 1 : -1);
}

