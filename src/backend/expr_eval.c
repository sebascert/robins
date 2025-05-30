#include "backend/expr_eval.h"

#include "backend/arithmetic.h"
#include "semantic/analyzer.h"

struct literal op_unary_arithmetic(const struct operation *node);
struct literal op_binary_arithmetic(const struct operation *node);

struct literal expr_eval(const struct astnode *node)
{
    if (!node)
        return NULL_LITERAL;

    switch (node->type) {
        case ASTNODE_T_LITERAL:
            return node->lit;
        case ASTNODE_T_OPERATION:
            switch (node->op.type) {
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
                        goto invalid_ast;
                    }
                    return op_binary_arithmetic(&node->op);
                unary:
                    if (node->op.n_operands != 1) {
                        goto invalid_ast;
                    }
                    return op_unary_arithmetic(&node->op);
                default:
                    goto invalid_ast;
            }
        default:
            break;
    }
invalid_ast:
    return NULL_LITERAL;
}

struct literal op_unary_arithmetic(const struct operation *node)
{
    struct literal a;
    a = expr_eval(node->operands[0]);
    switch (node->type) {
        case '-':
            return negate_lit(a);
        default:
            return NULL_LITERAL;
    }
}

struct literal op_binary_arithmetic(const struct operation *node)
{
    struct literal a, b, result;
    a = expr_eval(node->operands[0]);
    b = expr_eval(node->operands[1]);

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
