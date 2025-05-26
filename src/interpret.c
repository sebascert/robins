#include "interpret.h"
#include "parse.h"
#include "var.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void initialize_interpreter(void) {}

// interpret

// operations
struct var op_unary_arithmetic(const struct op_node *node, FILE *fout);
struct var op_binary_arithmetic(const struct op_node *node, FILE *fout);

struct var interpret_ast_node(const struct ast_node *node, FILE *fout) {
    if (!node)
        return NULL_VAR;

    switch (node->type) {
    case NTYPE_CONST:
        return node->cst.v;
    case NTYPE_OP:
        switch (node->op.op) {
        /* arithmetic */
        case '-':
            if (node->op.n_operands == 1) {
                goto unary;
            }
        case '+':
        case '*':
        case '/':
            if (node->op.n_operands != 2) {
                sserror(
                    false,
                    "interpret_ast_node: binary arithmetic expects 2 operands");
                return NULL_VAR;
            }
            return op_binary_arithmetic(&node->op, fout);
        unary:
            if (node->op.n_operands != 1) {
                sserror(
                    false,
                    "interpret_ast_node: unary arithmetic expects 1 operand");
                return NULL_VAR;
            }
            return op_unary_arithmetic(&node->op, fout);
        default:
            sserror(true, "interpret_ast_node: invalid operand\n");
            return NULL_VAR;
        }
    default:
        sserror(true, "interpret_ast_node: invalid node type\n");
        return NULL_VAR;
    }
}

void interpret_ast(const struct ast_node *ast, FILE *fout) {
    if (!ast) {
        sserror(true, "interpret_ast: null ast\n");
        return;
    }

    struct var result = interpret_ast_node(ast, fout);

    // print the result of an expression if not null
    if (result.type == VTYPE_NULL)
        return;

    fprintf(fout, "= ");
    print_var(result, fout);
    fprintf(fout, "\n");
}

struct var op_unary_arithmetic(const struct op_node *node, FILE *fout) {
    struct var a;
    a = interpret_ast_node(node->operands[0], fout);
    switch (node->op) {
    case '-':
        return negate_var(a);
    default:
        return NULL_VAR;
    }
}

struct var op_binary_arithmetic(const struct op_node *node, FILE *fout) {
    struct var a, b, result;
    a = interpret_ast_node(node->operands[0], fout);
    b = interpret_ast_node(node->operands[1], fout);

    switch (node->op) {
    case '-':
        return sub_var(a, b);
    case '+':
        return add_var(a, b);
    case '*':
        return mul_var(a, b);
    case '/':
        result = div_var(a, b);
        if (result.type == VTYPE_NULL)
            sserror(false, "op_binary_arithmetic: division by zero\n");

        return result;
    default:
        return NULL_VAR;
    }
}

void sserror(bool terminate, const char *s, ...) {
    va_list args;

    va_start(args, s);
    fprintf(stderr, "semantic error: ");
    if (s)
        vfprintf(stderr, s, args);
    va_end(args);

    if (terminate)
        exit(1);
}
