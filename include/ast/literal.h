#ifndef AST_LITERAL_H
#define AST_LITERAL_H

#include <stdio.h>

extern const char *const literal_t_names[];

typedef enum {
    LITERAL_T_NULL = -1,
    LITERAL_T_INT,
    LITERAL_T_REAL,
} literal_t;

union literal_v {
    int ival;
    double rval;
};

struct literal {
    literal_t type;
    union literal_v val;
};

extern struct literal NULL_LITERAL;

// no null checks for types performed

// utils
int print_lit(const struct literal *lit, FILE *fout);

int cast_lit(struct literal *lit, literal_t cast);
union literal_v literal_default(literal_t type);

// arithmetic
struct literal negate_lit(struct literal a);
struct literal add_lit(struct literal a, struct literal b);
struct literal sub_lit(struct literal a, struct literal b);
struct literal mul_lit(struct literal a, struct literal b);
struct literal div_lit(struct literal a, struct literal b);

#endif /* ifndef AST_LITERAL_H */
