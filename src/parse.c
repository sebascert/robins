#include "parse.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_node(struct ast_node *node) {
    if (!node) {
        yyerror("free_node received null node");
        return;
    }

    switch (node->type) {
    case NTYPE_OP:
        for (size_t i = 0; i < node->op.n_operands; i++) {
            free_node(node->op.operands[i]);
        }
        free(node->op.operands);
        free(node);
        break;
    case NTYPE_CONST:
        free(node);
        break;
    }
}

struct ast_node *push_op(int op, size_t n_operands, ...) {
    struct ast_node *node = malloc(sizeof(struct ast_node));
    if (!node) {
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->op.operands = malloc(sizeof(struct ast_node *) * n_operands);
    if (!node->op.operands) {
        free(node);
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->op.op = op;
    node->op.n_operands = n_operands;

    va_list args;
    va_start(args, n_operands);
    for (int i = 0; i < n_operands; i++) {
        node->op.operands[i] = va_arg(args, struct ast_node *);
    }
    va_end(args);

    node->type = NTYPE_OP;
    return node;
}

struct ast_node *push_const(var_type type, union var_val val) {
    struct ast_node *node = malloc(sizeof(struct ast_node));
    if (!node) {
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->cst.v.type = type;
    node->cst.v.val = val;

    node->type = NTYPE_CONST;
    return node;
}

void yyerror(const char *s) {
    extern int yylineno;
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}
