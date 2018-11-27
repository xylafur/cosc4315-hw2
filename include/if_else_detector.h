
#ifndef IF_ELSE_DETECTOR_H
#define IF_ELSE_DETECTOR_H

#include "ast.h"
#include <queue>
#include <vector>
#include <assert.h>
#include <iostream>

/* Main detect-if-else function which returns a vector of nested levels
 * of if statements in the source code */
std::vector<int> detect_if_else(ParseTreeNode *program_node);
// this function detects the nested levels of all root if-else nodes in the
// entire parse tree
//
// params: @program_node - a ParseTreeNode of type PROGRAM_NODE
// returns: a vector of ints containing the nesting levels of all if statements
// in the program in order of where they appear in the code
//



/*
 * BELOW ARE UTILITIES THAT THE FUNCTION detect_if_else uses
 * */
std::vector<ParseTreeNode*> find_root_if_nodes(ParseTreeNode *start);
// Is used by detect_if_else as a utility. You should not call function.
//
// this function traverses through the children of @start and the children
// of func def nodes in @start to find all the root if statement nodes 
// 
// params : @start - a parse tree node either of type BLOCK_STMT_NODE or of
//                   type PROGRAM_NODE
//
// returns: a vector containing pointers to nodes of type
//          BRANCH_STMT_ELSE_NODE or BRANCH_STMT_NO_ELSE_NODE
//
// usage: for optimized usage of the c++ vector initializer, call it like this
//
// {
//    vector<ParseTreeNode*> root_ifs = find_root_if_nodes(program_node):
// }
//    // initialize it like this ^ and not like this V
// {
//    vector<ParseTreeNode*> root_ifs;
//    root_ifs = find_root_if_nodes(program_node):
// }
//

int count_if_children(ParseTreeNode *if_node);
// used by detect_if_else as a utility, You should not call this function
//
// returns the maximum nested if else depth of the given @if_node
//
// params : @if_node - a parse tree node either of type BLOCK_STMT_NODE or of
//                   type PROGRAM_NODE
//
// returns: the max depth of if_else nodes


#endif

