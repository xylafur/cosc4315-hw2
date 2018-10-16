
#ifndef PARSER_TREE_NODE_H
#define PARSER_TREE_NODE_H

#include <cstdint.h>

struct ParseTreeNode {
    ParseTreeNode(tokens token_type) {
        this->token_type = token_type;
    }

    ~ParseTreeNode() {
        if (is_string && const_data.str_const != nullptr) {
            delete[] const_data.str_const;
        }

        // comment this out to delete manually rather than recursively
        for (size_t i = 0; i < n_children; ++i) delete children[i];
    }

    int token_type = 0;

    size_t n_children = 0;
    ParseTreeNode *children = nullptr;

    union const_data { // for if this is a constant
        int64_t i_const = 0;
        char *str_const = nullptr;
        char *indent_const = nullptr;
    };
};



#endif

