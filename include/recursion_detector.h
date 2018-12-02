
#ifndef RECURSION_DETECTOR_H
#define RECURSION_DETECTOR_H

#include <queue>
#include <vector>
#include <string>
#include <cstring>
#include "ast.h"

vector<ParseTreeNode*> find_recursive_functions(ParseTreeNode *node);

bool is_recursive_function(ParseTreeNode *func_node);

#endif

