#include "interpret.h"
#include "ast/literal.h"
#include "ast/node.h"
#include "frontend/yyshared.h"

#include <stdarg.h>
#include <stdio.h>

void initialize_interpreter(void) {}

// operations
struct literal op_unary_arithmetic(const struct operation *node, FILE *fout);
struct literal op_binary_arithmetic(const struct operation *node, FILE *fout);

struct literal interpret_ast_node(const struct astnode *node, FILE *fout) {
    if (!node)
        return NULL_LITERAL;

    switch (node->type) {
    case ASTNODE_T_LITERAL:
        return node->lit;
    case ASTNODE_T_OPERATION:
        switch (node->op.type) {
        /* arithmetic */
        case '-':
            if (node->op.n_operands == 1) {
                goto unary;
            }
            goto binary;
        case '+':
        case '*':
        case '/':
        binary:
            if (node->op.n_operands != 2) {
                sserror(
                    "interpret_ast_node: binary arithmetic expects 2 operands");
                return NULL_LITERAL;
            }
            return op_binary_arithmetic(&node->op, fout);
        unary:
            if (node->op.n_operands != 1) {
                sserror(
                    "interpret_ast_node: unary arithmetic expects 1 operand");
                return NULL_LITERAL;
            }
            return op_unary_arithmetic(&node->op, fout);
        default:
            sserror("interpret_ast_node: invalid operand\n");
            return NULL_LITERAL;
        }
    default:
        sserror("interpret_ast_node: invalid node type\n");
        return NULL_LITERAL;
    }
}

FILE *interpret_ast_out;
void interpret_ast(const struct astnode *ast) {
    if (!ast) {
        sserror("interpret_ast: null ast\n");
        return;
    }

    struct literal result = interpret_ast_node(ast, interpret_ast_out);

    // print the result of an expression if not null
    if (result.type == LITERAL_T_NULL)
        return;

    fprintf(interpret_ast_out, "= ");
    print_lit(&result, interpret_ast_out);
    fprintf(interpret_ast_out, "\n");
}

struct literal op_unary_arithmetic(const struct operation *node, FILE *fout) {
    struct literal a;
    a = interpret_ast_node(node->operands[0], fout);
    switch (node->type) {
    case '-':
        return negate_lit(a);
    default:
        return NULL_LITERAL;
    }
}

struct literal op_binary_arithmetic(const struct operation *node, FILE *fout) {
    struct literal a, b, result;
    a = interpret_ast_node(node->operands[0], fout);
    b = interpret_ast_node(node->operands[1], fout);

    switch (node->type) {
    case '-':
        return sub_lit(a, b);
    case '+':
        return add_lit(a, b);
    case '*':
        return mul_lit(a, b);
    case '/':
        result = div_lit(a, b);
        if (result.type == LITERAL_T_NULL)
            sserror("op_binary_arithmetic: division by zero\n");

        return result;
    default:
        return NULL_LITERAL;
    }
}

void sserror(const char *s, ...) {
    va_list args;

    va_start(args, s);
    fprintf(stderr, "%s:%d:  %s\n", yyfilename, yylineno, s);
    if (s)
        vfprintf(stderr, s, args);
    va_end(args);
}
