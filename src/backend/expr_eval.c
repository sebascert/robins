#include "backend/expr_eval.h"

#include "backend/arithmetic.h"
#include "backend/errors.h"
#include "semantic/error.h"

int op_unary_arithmetic(const struct operation *node, struct literal *result);
int op_binary_arithmetic(const struct operation *node, struct literal *result);

int expr_eval(const struct astnode *node, struct literal *result)
{
    if (!node)
        goto invalid_ast;

    switch (node->type) {
        case ASTNODE_T_LITERAL:
            *result = node->lit;
            return 0;
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
                    return op_binary_arithmetic(&node->op, result);
                unary:
                    if (node->op.n_operands != 1) {
                        goto invalid_ast;
                    }
                    return op_unary_arithmetic(&node->op, result);
                default:
                    goto invalid_ast;
            }
        default:
            goto invalid_ast;
    }
invalid_ast:
    *result = NULL_LITERAL;
    return INVALID_AST_ERROR;
}

int op_unary_arithmetic(const struct operation *node, struct literal *result)
{
    int error;
    struct literal a;
    if ((error = expr_eval(node->operands[0], &a)) != 0)
        return error;

    switch (node->type) {
        case '-':
            *result = negate_lit(a);
            break;
        default:
            return INVALID_AST_ERROR;
    }
    return 0;
}

int op_binary_arithmetic(const struct operation *node, struct literal *result)
{
    int error;
    struct literal a, b;
    if ((error = expr_eval(node->operands[0], &a)) != 0)
        return error;
    if ((error = expr_eval(node->operands[1], &b)) != 0)
        return error;

    switch (node->type) {
        case '-':
            *result = sub_lit(a, b);
            break;
        case '+':
            *result = add_lit(a, b);
            break;
        case '*':
            *result = mul_lit(a, b);
            break;
        case '/':
            *result = div_lit(a, b);
            if (result->type != LITERAL_T_NULL)
                break;

            sserror("division by zero\n");
            return ARITHMETIC_ERROR;
        default:
            return INVALID_AST_ERROR;
    }
    return 0;
}
