#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#include "var.h"

struct ast_node;

typedef void (*ast_evaluator)(const struct ast_node *);
extern ast_evaluator eval_ast_statement;

typedef enum {
    NTYPE_ID,
    NTYPE_OP,
    NTYPE_CONST,
} node_type;

struct id_node {
    char *id;
};

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
        struct id_node id;
        struct op_node op;
        struct cst_node cst;
    };
};

void free_node(struct ast_node *node);

struct ast_node *push_id(const char *id);
struct ast_node *push_op(int op, size_t n_operands, ...);
struct ast_node *push_const(var_type type, union var_val val);

void yyerror(const char *s);

#endif /* ifndef PARSER_H */
