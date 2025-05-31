#ifndef AST_NODES_INSTRUCTION_H
#define AST_NODES_INSTRUCTION_H

#include "ast/user_types.h"

#include <stddef.h>

struct astnode;
struct instruction {
    instruction_t type;
    size_t n_args;
    struct astnode **args;
};

#endif /* ifndef AST_NODES_INSTRUCTION_H */
