#ifndef AST_OPERATION_H
#define AST_OPERATION_H

#include <stddef.h>

extern const char *const operation_t_names[];

typedef enum {
    OPERATION_T_ADD = '+',
    OPERATION_T_sub = '-',
    OPERATION_T_mul = '*',
    OPERATION_T_div = '/',
} operation_t;

struct astnode;
struct operation {
    operation_t type;
    size_t n_operands;
    struct astnode **operands;
};

#endif /* ifndef AST_OPERATION_H */
