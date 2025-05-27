#include "backend/translator.h"

#include "ast/literal.h"
#include "ast/node.h"
#include "backend/expr_eval.h"
#include "semantic/analyzer.h"

#include <stdio.h>

// operations
struct literal op_unary_arithmetic(const struct operation *node, FILE *fout);
struct literal op_binary_arithmetic(const struct operation *node, FILE *fout);

struct literal translate_ast_node(const struct astnode *node, FILE *fout) {
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
                sserror("binary arithmetic expects 2 operands");
                return NULL_LITERAL;
            }
            return op_binary_arithmetic(&node->op, fout);
        unary:
            if (node->op.n_operands != 1) {
                sserror("unary arithmetic expects 1 operand");
                return NULL_LITERAL;
            }
            return op_unary_arithmetic(&node->op, fout);
        default:
            sserror("invalid operand\n");
            return NULL_LITERAL;
        }
    default:
        sserror("invalid node type\n");
        return NULL_LITERAL;
    }
}

FILE *translate_ast_out;
void translate_ast(const struct astnode *ast) {
    if (!ast) {
        sserror("translate_ast: null ast\n");
        return;
    }

    struct literal result = translate_ast_node(ast, translate_ast_out);

    // print the result of an expression if not null
    if (result.type == LITERAL_T_NULL)
        return;

    fprintf(translate_ast_out, "= ");
    print_lit(&result, translate_ast_out);
    fprintf(translate_ast_out, "\n");
}

struct literal op_unary_arithmetic(const struct operation *node, FILE *fout) {
    struct literal a;
    a = translate_ast_node(node->operands[0], fout);
    switch (node->type) {
    case '-':
        return negate_lit(a);
    default:
        return NULL_LITERAL;
    }
}

struct literal op_binary_arithmetic(const struct operation *node, FILE *fout) {
    struct literal a, b, result;
    a = translate_ast_node(node->operands[0], fout);
    b = translate_ast_node(node->operands[1], fout);

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
            sserror("division by zero\n");

        return result;
    default:
        return NULL_LITERAL;
    }
}
