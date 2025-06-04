#include "ast/nodes/literal.h"

#include <stdio.h>

const char *const literal_t_names[] = {
    [LITERAL_T_INT] = "int",
    [LITERAL_T_REAL] = "double",
};

struct literal NULL_LITERAL = {
    .type = LITERAL_T_NULL,
    .val = {0},
};

int print_lit(const struct literal *lit, FILE *fout)
{
    switch (lit->type) {
        case LITERAL_T_INT:
            fprintf(fout, "%d", lit->val.ival);
            return 0;
        case LITERAL_T_REAL:
            fprintf(fout, "%.2lf", lit->val.rval);
            return 0;
        default:
            return 1;
    }
}
