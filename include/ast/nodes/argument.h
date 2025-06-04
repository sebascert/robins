#ifndef AST_NODES_ARGUMENT_H
#define AST_NODES_ARGUMENT_H

#include "ast/user_types.h"

struct astnode;
struct argument {
    argument_t type;
    struct astnode *resolution;
};

#endif /* ifndef AST_NODES_ARGUMENT_H */
