#ifndef AST_NODES_LITERAL_H
#define AST_NODES_LITERAL_H

#include <stdio.h>

extern const char *const literal_t_names[];

typedef enum {
    LITERAL_T_NULL = -1,
    LITERAL_T_INT,
    LITERAL_T_REAL,
} literal_t;

typedef int literal_t_int;
typedef double literal_t_real;

union literal_v {
    literal_t_int ival;
    literal_t_real rval;
};

struct literal {
    literal_t type;
    union literal_v val;
};

extern struct literal NULL_LITERAL;

int print_lit(const struct literal *lit, FILE *fout);

#endif /* ifndef AST_NODES_LITERAL_H */
