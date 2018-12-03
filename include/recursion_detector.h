
#ifndef RECURSION_DETECTOR_H
#define RECURSION_DETECTOR_H

#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include "ast.h"

bool contains_func_call_of(ParseTreeNode *func_node, const char *func_name);

bool is_recursive_function(ParseTreeNode *func_def);

bool is_base_return_stmt(ParseTreeNode *ret_stmt, const char *func_name);

std::vector<ParseTreeNode*>
find_return_statements(ParseTreeNode *func_def);

bool recursive_function_terminates
(ParseTreeNode * rec_func, ParseTreeNode * call);

std::vector<ParseTreeNode*> find_global_func_calls
(ParseTreeNode* program_node, std::vector<ParseTreeNode*> &recursive_functions);

std::vector<ParseTreeNode*> find_recursive_functions(ParseTreeNode *start);

void clean_curr_childs(ParseTreeNode *node);

ParseTreeNode * find_shallowest_func_call(ParseTreeNode *node, const char * func_name);

long condition_evaluator(ParseTreeNode *cond, int val);

long evaluate_expr(ParseTreeNode *expr, int val);




#endif

