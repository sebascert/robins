#ifndef BACKEND_ARITHMETIC_H
#define BACKEND_ARITHMETIC_H

#include "ast/nodes/literal.h"

int cast_lit(struct literal *lit, literal_t cast);
union literal_v literal_default(literal_t type);

struct literal negate_lit(struct literal a);
struct literal add_lit(struct literal a, struct literal b);
struct literal sub_lit(struct literal a, struct literal b);
struct literal mul_lit(struct literal a, struct literal b);
struct literal div_lit(struct literal a, struct literal b);

#endif /* ifndef BACKEND_ARITHMETIC_H */
