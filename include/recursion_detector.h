
#ifndef RECURSION_DETECTOR_H
#define RECURSION_DETECTOR_H

#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <cstring>
#include "ast.h"

bool recursive_function_terminates(ParseTreeNode * rec_func,
                                   ParseTreeNode * call);
std::vector<ParseTreeNode*> find_recursive_functions(ParseTreeNode *node);
std::vector<ParseTreeNode*> find_global_func_calls(ParseTreeNode* program_node);

bool is_recursive_function(ParseTreeNode *func_node);

#endif

