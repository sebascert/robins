#ifndef AST_NODE_H
#define AST_NODE_H

#include "ast/literal.h"
#include "ast/operation.h"

#include <stddef.h>

extern const char *astnode_t_names[];

typedef enum {
    ASTNODE_T_OPERATION,
    ASTNODE_T_LITERAL,
} astnode_t;

struct astnode {
    astnode_t type;
    union {
        struct operation op;
        struct literal lit;
    };
};

void free_node(struct astnode *node);

struct astnode *push_op(operation_t op, size_t n_operands, ...);
struct astnode *push_literal(literal_t type, union literal_v val);

void yyerror(const char *s);

#endif /* ifndef AST_NODE_H */
