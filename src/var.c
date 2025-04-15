#include "var.h"

#include <stdio.h>

const char *const vtype_names[] = {
    [VTYPE_INT] = "int",
    [VTYPE_FLOAT] = "float",
};

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
        fprintf(fout, "%.2f", var.val.fval);
        return 0;
    default:
        return 1;
    }
}

int cast_var(struct var *var, var_type cast) {
    if (var->type == cast)
        return 0;

    switch (var->type) {
    case VTYPE_INT:
        if (cast == VTYPE_FLOAT) {
            var->val.fval = (float)var->val.ival;
            goto casted;
        }
        return 1;

    case VTYPE_FLOAT:
        if (cast == VTYPE_INT) {
            var->val.ival = (int)var->val.fval;
            goto casted;
        }
        return 1;

    default:
        return 1;
    }
casted:
    var->type = cast;
    return 0;
}

union var_val type_default_val(var_type type) {
    switch (type) {
    case VTYPE_INT:
        return (union var_val){.ival = 0};

    case VTYPE_FLOAT:
        return (union var_val){.fval = 0.0f};

    default:
        return (union var_val)0;
    }
}

struct var negate_var(struct var a) {
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

struct var add_var(struct var a, struct var b) {
    if (a.type == VTYPE_FLOAT || b.type == VTYPE_FLOAT) {
        float lhs = a.type == VTYPE_FLOAT ? a.val.fval : (float)a.val.ival;
        float rhs = b.type == VTYPE_FLOAT ? b.val.fval : (float)b.val.ival;
        return (struct var){VTYPE_FLOAT, .val.fval = lhs + rhs};
    } else {
        return (struct var){VTYPE_INT, .val.ival = a.val.ival + b.val.ival};
    }
}

struct var sub_var(struct var a, struct var b) {
    if (a.type == VTYPE_FLOAT || b.type == VTYPE_FLOAT) {
        float lhs = a.type == VTYPE_FLOAT ? a.val.fval : (float)a.val.ival;
        float rhs = b.type == VTYPE_FLOAT ? b.val.fval : (float)b.val.ival;
        return (struct var){VTYPE_FLOAT, .val.fval = lhs - rhs};
    } else {
        return (struct var){VTYPE_INT, .val.ival = a.val.ival - b.val.ival};
    }
}

struct var mul_var(struct var a, struct var b) {
    if (a.type == VTYPE_FLOAT || b.type == VTYPE_FLOAT) {
        float lhs = a.type == VTYPE_FLOAT ? a.val.fval : (float)a.val.ival;
        float rhs = b.type == VTYPE_FLOAT ? b.val.fval : (float)b.val.ival;
        return (struct var){VTYPE_FLOAT, .val.fval = lhs * rhs};
    } else {
        return (struct var){VTYPE_INT, .val.ival = a.val.ival * b.val.ival};
    }
}

struct var div_var(struct var a, struct var b) {
    float lhs = a.type == VTYPE_FLOAT ? a.val.fval : (float)a.val.ival;
    float rhs = b.type == VTYPE_FLOAT ? b.val.fval : (float)b.val.ival;

    if (rhs == 0)
        return NULL_VAR;
    return (struct var){VTYPE_FLOAT, .val.fval = lhs / rhs};
}
