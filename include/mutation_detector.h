
#ifndef MUTATION_DETECTOR_H
#define MUTATION_DETECTOR_H

#include "ast.h"
#include <queue>
#include <vector>
#include <unordered_map>
#include <set>
#include <assert.h>

std::vector<std::string> find_mutations(ParseTreeNode *program_node);

std::unordered_map<std::string, int>
find_assignments(ParseTreeNode *current_scope, std::string prefix);
// utility used by find_mutations

#endif

