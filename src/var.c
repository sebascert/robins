#include "var.h"

#include <stdio.h>

struct var NULL_VAR = {
    .type = VTYPE_NULL,
    .val = {0},
};

int print_var(struct var var, FILE *fout) {
    switch (var.type) {
    case VTYPE_INT:
        fprintf(fout, "%d", var.val.ival);
        return 0;
    case VTYPE_FLOAT:
        fprintf(fout, "%f", var.val.fval);
        return 0;
    default:
        return 1;
    }
}

struct var var_negate(struct var a) {
    struct var res = a;
    if (a.type == VTYPE_INT) {
        res.val.ival = -a.val.ival;
    } else if (a.type == VTYPE_FLOAT) {
        res.val.fval = -a.val.fval;
    } else {
        return NULL_VAR;
    }

    return res;
}

struct var var_add(struct var a, struct var b) {
    if (a.type == VTYPE_FLOAT || b.type == VTYPE_FLOAT) {
        float lhs = a.type == VTYPE_FLOAT ? a.val.fval : (float)a.val.ival;
        float rhs = b.type == VTYPE_FLOAT ? b.val.fval : (float)b.val.ival;
        return (struct var){VTYPE_FLOAT, .val.fval = lhs + rhs};
    } else {
        return (struct var){VTYPE_INT, .val.ival = a.val.ival + b.val.ival};
    }
}

struct var var_sub(struct var a, struct var b) {
    if (a.type == VTYPE_FLOAT || b.type == VTYPE_FLOAT) {
        float lhs = a.type == VTYPE_FLOAT ? a.val.fval : (float)a.val.ival;
        float rhs = b.type == VTYPE_FLOAT ? b.val.fval : (float)b.val.ival;
        return (struct var){VTYPE_FLOAT, .val.fval = lhs - rhs};
    } else {
        return (struct var){VTYPE_INT, .val.ival = a.val.ival - b.val.ival};
    }
}

struct var var_mul(struct var a, struct var b) {
    if (a.type == VTYPE_FLOAT || b.type == VTYPE_FLOAT) {
        float lhs = a.type == VTYPE_FLOAT ? a.val.fval : (float)a.val.ival;
        float rhs = b.type == VTYPE_FLOAT ? b.val.fval : (float)b.val.ival;
        return (struct var){VTYPE_FLOAT, .val.fval = lhs * rhs};
    } else {
        return (struct var){VTYPE_INT, .val.ival = a.val.ival * b.val.ival};
    }
}

struct var var_div(struct var a, struct var b) {
    float lhs = a.type == VTYPE_FLOAT ? a.val.fval : (float)a.val.ival;
    float rhs = b.type == VTYPE_FLOAT ? b.val.fval : (float)b.val.ival;

    if (rhs == 0)
        return NULL_VAR;
    return (struct var){VTYPE_FLOAT, .val.fval = lhs / rhs};
}
