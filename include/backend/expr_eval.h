#ifndef BACKEND_EXPR_EVAL_H
#define BACKEND_EXPR_EVAL_H

#include "ast/literal.h"

int cast_lit(struct literal *lit, literal_t cast);
union literal_v literal_default(literal_t type);

// arithmetic
struct literal negate_lit(struct literal a);
struct literal add_lit(struct literal a, struct literal b);
struct literal sub_lit(struct literal a, struct literal b);
struct literal mul_lit(struct literal a, struct literal b);
struct literal div_lit(struct literal a, struct literal b);

#endif /* ifndef BACKEND_EXPR_EVAL_H */
