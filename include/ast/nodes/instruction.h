#ifndef AST_NODES_INSTRUCTION_H
#define AST_NODES_INSTRUCTION_H

#include <stddef.h>

extern const char *const instruction_t_mnemonics[];

typedef enum {
    INS_ROTATE, /* macroized */
} instruction_t;

struct astnode;
struct instruction {
    instruction_t type;
    size_t n_args;
    struct astnode **args;
};

#endif /* ifndef AST_NODES_INSTRUCTION_H */
