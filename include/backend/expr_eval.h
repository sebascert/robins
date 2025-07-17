#ifndef BACKEND_EXPR_EVAL_H
#define BACKEND_EXPR_EVAL_H

#include "ast/node.h"

#include <stdbool.h>

// evaluate expression in AST
struct literal expr_eval(const struct astnode *expr);

#endif /* ifndef BACKEND_EXPR_EVAL_H */
