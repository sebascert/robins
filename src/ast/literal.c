#include "ast/literal.h"

#include <stdio.h>

const char *const literal_t_names[] = {
    [LITERAL_T_INT] = "int",
    [LITERAL_T_REAL] = "float",
};

struct literal NULL_LITERAL = {
    .type = LITERAL_T_NULL,
    .val = {0},
};

int print_lit(const struct literal *lit, FILE *fout) {
    switch (lit->type) {
    case LITERAL_T_INT:
        fprintf(fout, "%d", lit->val.ival);
        return 0;
    case LITERAL_T_REAL:
        fprintf(fout, "%.2f", lit->val.rval);
        return 0;
    default:
        return 1;
    }
}

int cast_lit(struct literal *lit, literal_t cast) {
    if (lit->type == cast)
        return 0;

    switch (lit->type) {
    case LITERAL_T_INT:
        if (cast == LITERAL_T_REAL) {
            lit->val.rval = (float)lit->val.ival;
            goto casted;
        }
        return 1;

    case LITERAL_T_REAL:
        if (cast == LITERAL_T_INT) {
            lit->val.ival = (int)lit->val.rval;
            goto casted;
        }
        return 1;

    default:
        return 1;
    }
casted:
    lit->type = cast;
    return 0;
}

union literal_v literal_default(literal_t type) {
    switch (type) {
    case LITERAL_T_INT:
        return (union literal_v){.ival = 0};

    case LITERAL_T_REAL:
        return (union literal_v){.rval = 0.0f};

    default:
        return (union literal_v)0;
    }
}

struct literal negate_lit(struct literal a) {
    struct literal res = a;
    if (a.type == LITERAL_T_INT) {
        res.val.ival = -a.val.ival;
    } else if (a.type == LITERAL_T_REAL) {
        res.val.rval = -a.val.rval;
    } else {
        return NULL_LITERAL;
    }

    return res;
}

struct literal add_lit(struct literal a, struct literal b) {
    if (a.type == LITERAL_T_REAL || b.type == LITERAL_T_REAL) {
        float lhs = a.type == LITERAL_T_REAL ? a.val.rval : (float)a.val.ival;
        float rhs = b.type == LITERAL_T_REAL ? b.val.rval : (float)b.val.ival;
        return (struct literal){LITERAL_T_REAL, .val.rval = lhs + rhs};
    } else {
        return (struct literal){LITERAL_T_INT,
                                .val.ival = a.val.ival + b.val.ival};
    }
}

struct literal sub_lit(struct literal a, struct literal b) {
    if (a.type == LITERAL_T_REAL || b.type == LITERAL_T_REAL) {
        float lhs = a.type == LITERAL_T_REAL ? a.val.rval : (float)a.val.ival;
        float rhs = b.type == LITERAL_T_REAL ? b.val.rval : (float)b.val.ival;
        return (struct literal){LITERAL_T_REAL, .val.rval = lhs - rhs};
    } else {
        return (struct literal){LITERAL_T_INT,
                                .val.ival = a.val.ival - b.val.ival};
    }
}

struct literal mul_lit(struct literal a, struct literal b) {
    if (a.type == LITERAL_T_REAL || b.type == LITERAL_T_REAL) {
        float lhs = a.type == LITERAL_T_REAL ? a.val.rval : (float)a.val.ival;
        float rhs = b.type == LITERAL_T_REAL ? b.val.rval : (float)b.val.ival;
        return (struct literal){LITERAL_T_REAL, .val.rval = lhs * rhs};
    } else {
        return (struct literal){LITERAL_T_INT,
                                .val.ival = a.val.ival * b.val.ival};
    }
}

struct literal div_lit(struct literal a, struct literal b) {
    float lhs = a.type == LITERAL_T_REAL ? a.val.rval : (float)a.val.ival;
    float rhs = b.type == LITERAL_T_REAL ? b.val.rval : (float)b.val.ival;

    if (rhs == 0)
        return NULL_LITERAL;
    return (struct literal){LITERAL_T_REAL, .val.rval = lhs / rhs};
}
