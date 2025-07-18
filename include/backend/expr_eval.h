#ifndef BACKEND_EXPR_EVAL_H
#define BACKEND_EXPR_EVAL_H

#include "ast/node.h"

#include <stdbool.h>

// evaluate expression in AST
int expr_eval(const struct astnode *node, struct literal *result);

#endif /* ifndef BACKEND_EXPR_EVAL_H */
