#ifndef AST_NODE_H
#define AST_NODE_H

#include <stddef.h>

#include "ast/literal.h"

struct ast_node;

typedef enum {
    NTYPE_OP,
    NTYPE_CONST,
} node_type;

struct op_node {
    int op;
    size_t n_operands;
    struct ast_node **operands;
};

struct cst_node {
    struct var v;
};

struct ast_node {
    node_type type;
    union {
        struct op_node op;
        struct cst_node cst;
    };
};

void free_node(struct ast_node *node);

struct ast_node *push_op(int op, size_t n_operands, ...);
struct ast_node *push_const(var_type type, union var_val val);

void yyerror(const char *s);

#endif /* ifndef AST_NODE_H */
