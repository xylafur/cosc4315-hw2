
#include "if_else_detector.h"
using namespace std;

vector<int> detect_if_else(ParseTreeNode *program_node) {
    // sanity checks
    assert(program_node);
    assert(program_node->type == PROGRAM_NODE);

    vector<int> nested_levels;
        // stores the nesting level of the rudimentary if-else block

    vector<ParseTreeNode*> root_if_nodes = find_root_if_nodes(program_node);
        // find all root-if nodes in the code

    // for all root-if nodes in the program, count the nested levels
    for (ParseTreeNode *temp : root_if_nodes) {
        int count = count_if_children(temp);
        nested_levels.push_back(count);
    }

    // return result
    return nested_levels;
}

vector<ParseTreeNode*> find_root_if_nodes(ParseTreeNode *start) {

    // sanity checks
    assert(start);
    assert(start->type == BLOCK_STMT_NODE || start->type == PROGRAM_NODE);

    vector<ParseTreeNode*> root_ifs;

    ParseTreeNode *temp;

    for (int i = 0; i < start->num_children; i++) {
        temp = start->children[i];
        if (temp->type == BRANCH_STMT_ELSE_NODE
                || temp->type == BRANCH_STMT_NO_ELSE_NODE)
        {
            // if this is a if-statment, push its address to root_ifs
            root_ifs.push_back(temp);

        } else if (temp->type == FUNC_DEF_NODE) {
            // else if it is a func_def node, recurse to find the root if-statement
            // nodes in its block

            vector<ParseTreeNode*> func_def_root_ifs = find_root_if_nodes(temp->children[0]);

            // insert values of func_def_root_ifs into root_ifs
            root_ifs.reserve(root_ifs.size() + func_def_root_ifs.size()); 
            root_ifs.insert( root_ifs.end(),
                             func_def_root_ifs.begin(),
                             func_def_root_ifs.end());
        }
    }

    return root_ifs;
}

int count_if_children(ParseTreeNode *if_node) {

    // sanity checks
    assert(if_node);
    assert(if_node->type == BRANCH_STMT_ELSE_NODE
           || if_node->type == BRANCH_STMT_NO_ELSE_NODE);

    queue<std::pair<ParseTreeNode *, int>> if_node_queue;
        // queue containing pairs of 
        // ParseTreeNode and the depth of that node relative to @if_node

    ParseTreeNode *temp = if_node;
    int depth = 0;
    std::pair<ParseTreeNode*, int> queue_pair = std::make_pair(temp, depth);

    if_node_queue.push(queue_pair);

    int max_depth = -1;

    while (!if_node_queue.empty())
    {
        // for convenience and readability
        queue_pair = if_node_queue.front(); if_node_queue.pop();
        temp = queue_pair.first;
        depth = queue_pair.second;

        if (max_depth < depth) max_depth = depth;

        // for-loop to process block nodes of if-statement node
        for (int i = 0; i < temp->num_children-1; i++)
        {

            ParseTreeNode* block_node = temp->children[i];

            // sanity check
            assert(block_node->type == BLOCK_STMT_NODE);

            // for-loop to process statements inside each block
            for (int j = 0; j < block_node->num_children; j++)
            {
                ParseTreeNode *child = block_node->children[j];
                assert(child);

                if (child->type == BRANCH_STMT_ELSE_NODE
                    || child->type == BRANCH_STMT_NO_ELSE_NODE)
                {
                    // push the paring if it is an if else node and increment the depth
                    if_node_queue.push(std::make_pair(child, depth+1));
                }
            }

        }
    }

    return max_depth;
}
