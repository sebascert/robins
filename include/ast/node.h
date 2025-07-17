#ifndef AST_NODE_H
#define AST_NODE_H

#include "ast/nodes/argument.h"
#include "ast/nodes/instruction.h"
#include "ast/nodes/literal.h"
#include "ast/nodes/operation.h"
#include "ast/nodes/statement.h"

#include <stddef.h>

extern const char *astnode_t_names[];

typedef enum {
    ASTNODE_T_STATEMENT,
    ASTNODE_T_INSTRUCTION,
    ASTNODE_T_ARGUMENT,
    ASTNODE_T_OPERATION,
    ASTNODE_T_LITERAL,
} astnode_t;

struct astnode {
    astnode_t type;
    union {
        struct statement stmt;
        struct instruction ins;
        struct argument arg;
        struct operation op;
        struct literal lit;
    };
};

void free_node(struct astnode *node);

// child nodes are not validated on node creation, as if the grammar is
// correct, no incorrect or unexpected situation should arise from the node
// creation usage. Still the AST walkers check for invalid cases in the AST for
// debugging.

struct astnode *push_stmt(struct astnode *first_ins);
struct astnode *stmt_append_ins(struct astnode *stmt, struct astnode *ins);

// parsed instructions and arguments should be handled equally for any user
// defined one, except for semantic validation.
struct astnode *push_ins(instruction_t ins, size_t n_args, ...);
struct astnode *push_arg(argument_t arg, struct astnode *resolution);
struct astnode *push_op(operation_t op, size_t n_operands, ...);
struct astnode *push_literal(literal_t type, union literal_v val);

void yyerror(const char *s);

#endif /* ifndef AST_NODE_H */
