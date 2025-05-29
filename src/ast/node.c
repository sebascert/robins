#include "ast/node.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *astnode_t_names[] = {
    [ASTNODE_T_OPERATION] = "op",
    [ASTNODE_T_LITERAL] = "lit",
};

void free_node(struct astnode *node)
{
    if (!node) {
        yyerror("free_node received null node");
        return;
    }

    switch (node->type) {
        case ASTNODE_T_OPERATION:
            for (size_t i = 0; i < node->op.n_operands; i++) {
                free_node(node->op.operands[i]);
            }
            free(node->op.operands);
            free(node);
            break;
        case ASTNODE_T_LITERAL:
            free(node);
            break;
    }
}

struct astnode *push_op(operation_t op, size_t n_operands, ...)
{
    struct astnode *node = malloc(sizeof(struct astnode));
    if (!node) {
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->op.operands = malloc(sizeof(struct astnode *) * n_operands);
    if (!node->op.operands) {
        free(node);
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->op.type = op;
    node->op.n_operands = n_operands;

    va_list args;
    va_start(args, n_operands);
    for (size_t i = 0; i < n_operands; i++) {
        node->op.operands[i] = va_arg(args, struct astnode *);
    }
    va_end(args);

    node->type = ASTNODE_T_OPERATION;
    return node;
}

struct astnode *push_literal(literal_t type, union literal_v val)
{
    struct astnode *node = malloc(sizeof(struct astnode));
    if (!node) {
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->lit.type = type;
    node->lit.val = val;

    node->type = ASTNODE_T_LITERAL;
    return node;
}
