
#include "mutation_detector.h"
using namespace std;

vector<string> find_mutations(ParseTreeNode *program_node) {
    auto assignments = find_assignments(program_node, std::string());
    vector<string> mutations;
    for (auto assn : assignments)
        if (assn.second > 1)
            mutations.push_back(assn.first);
    return mutations;
}

unordered_map<string, int>
find_assignments(ParseTreeNode *current_scope, string prefix) {

    assert(current_scope);
    assert(current_scope->type == PROGRAM_NODE
           || current_scope->type == BLOCK_STMT_NODE);

    unordered_map<string, int> assignments;
    ParseTreeNode *temp;

    for (int i = 0; i < current_scope->num_children; i++) {
        temp = current_scope->children[i];

        if (temp->type == ASSIGNMENT_NODE) {
            assert(temp->children[0]);

            string key = prefix + temp->children[0]->value.s_value;

            auto find_var = assignments.find(key);
            if (find_var != assignments.end()) {
                assignments[key] += 1;
            } else {
                assignments[key] = 1;
            }

        } else if (temp->type == BRANCH_STMT_ELSE_NODE
                    || temp->type == BRANCH_STMT_NO_ELSE_NODE) {

            for (int j = 0; j < temp->num_children-1; j++) {
                auto block_assignments = find_assignments(temp->children[j], prefix);

                for (auto a : block_assignments) {
                    auto find_a = assignments.find(a.first);
                    if (find_a != assignments.end())
                        assignments[a.first] += a.second;
                    else
                        assignments[a.first] = a.second;
                }
            }

        } else if (temp->type == FUNC_DEF_NODE) {
            string func_prefix = prefix + temp->value.s_value + "::";
            auto block_assignments = find_assignments(temp->children[0],func_prefix);

            set<string> param_names;
            for (int j = 0; j < temp->num_parameters; j++) {
                param_names.insert(func_prefix + temp->parameters[j]);
            }

            for (auto a : block_assignments) {
                auto find_a = assignments.find(a.first);
                if (find_a != assignments.end()) {
                    assignments[a.first] += a.second;
                } else {
                    assignments[a.first] = a.second;
                }

                // account for parameter mutation properly with +1
                auto is_param = param_names.find(a.first);
                if (is_param != param_names.end()) assignments[a.first] += 1;
            }
        }
    }

    return assignments;
}


