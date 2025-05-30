#ifndef AST_NODES_LITERAL_H
#define AST_NODES_LITERAL_H

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

int print_lit(const struct literal *lit, FILE *fout);

#endif /* ifndef AST_NODES_LITERAL_H */
