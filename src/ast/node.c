#include "ast/node.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *astnode_t_names[] = {
    [ASTNODE_T_STATEMENT] = "stmt", [ASTNODE_T_INSTRUCTION] = "ins",
    [ASTNODE_T_ARGUMENT] = "arg",   [ASTNODE_T_OPERATION] = "op",
    [ASTNODE_T_LITERAL] = "lit",
};

void free_node(struct astnode *node)
{
    if (!node) {
        yyerror("free_node received null node");
        return;
    }

    switch (node->type) {
        case ASTNODE_T_STATEMENT:
            for (size_t i = 0; i < node->stmt.n_ins; i++) {
                free_node(node->stmt.instructions[i]);
            }
            free(node->stmt.instructions);
            free(node);
            break;
        case ASTNODE_T_INSTRUCTION:
            for (size_t i = 0; i < node->ins.n_args; i++) {
                free_node(node->ins.args[i]);
            }
            free(node->ins.args);
            free(node);
            break;
        case ASTNODE_T_ARGUMENT:
            free(node->arg.resolution);
            free(node);
            break;
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
struct astnode *push_stmt(struct astnode *first_ins)
{
    struct astnode *node = malloc(sizeof(struct astnode));
    if (!node) {
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->stmt.instructions =
        malloc(sizeof(struct astnode *) * STMT_INIT_INS_SIZE);
    if (!node->stmt.instructions) {
        free(node);
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->stmt.n_ins = STMT_INIT_INS_SIZE;
    node->stmt.ins_vec_size = STMT_INIT_INS_SIZE;
    node->stmt.instructions[0] = first_ins;

    node->type = ASTNODE_T_STATEMENT;
    return node;
}

struct astnode *stmt_append_ins(struct astnode *stmt, struct astnode *ins)
{
    struct astnode **instructions = stmt->stmt.instructions;
    if (stmt->stmt.n_ins == stmt->stmt.ins_vec_size) {
        size_t next_size = stmt->stmt.ins_vec_size * 2;
        instructions =
            realloc(instructions, sizeof(struct astnode *) * next_size);
        if (!instructions) {
            yyerror("unable to allocate memory for AST nodes");
            return NULL;
        }

        stmt->stmt.ins_vec_size = next_size;
        stmt->stmt.instructions = instructions;
    }

    instructions[stmt->stmt.n_ins] = ins;
    stmt->stmt.n_ins++;
    return stmt;
}

struct astnode *push_ins(instruction_t ins, size_t n_args, ...)
{
    struct astnode *node = malloc(sizeof(struct astnode));
    if (!node) {
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->ins.args = malloc(sizeof(struct astnode *) * n_args);
    if (!node->ins.args) {
        free(node);
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->ins.type = ins;
    node->ins.n_args = n_args;

    va_list args;
    va_start(args, n_args);
    for (size_t i = 0; i < n_args; i++) {
        node->ins.args[i] = va_arg(args, struct astnode *);
    }
    va_end(args);

    node->type = ASTNODE_T_INSTRUCTION;
    return node;
}

struct astnode *push_arg(argument_t arg, struct astnode *resolution)
{
    struct astnode *node = malloc(sizeof(struct astnode));
    if (!node) {
        yyerror("unable to allocate memory for AST nodes");
        return NULL;
    }

    node->arg.type = arg;
    node->arg.resolution = resolution;

    node->type = ASTNODE_T_ARGUMENT;
    return node;
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
