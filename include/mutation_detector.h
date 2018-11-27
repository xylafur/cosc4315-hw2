
#ifndef MUTATION_DETECTOR_H
#define MUTATION_DETECTOR_H

#include "ast.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <set>
#include <assert.h>

std::vector<std::string> find_mutations(ParseTreeNode *program_node);
// this function returns the variable names of all the mutations in the code
// a variable is considered mutated if it is assigned more than once


std::unordered_map<std::string, int>
find_assignments(ParseTreeNode *current_scope, std::string prefix);
// utility used by find_mutations, DO NOT CALL

#endif

