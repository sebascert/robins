#ifndef VAR_H
#define VAR_H

#include <stdio.h>

extern const char *const vtype_names[];

typedef enum {
    VTYPE_NULL = -1,
    VTYPE_INT,
    VTYPE_FLOAT,
} var_type;

union var_val {
    int ival;
    float fval;
};

struct var {
    var_type type;
    union var_val val;
};

extern struct var NULL_VAR;

// no null checks for types performed

// utils
int print_var(struct var var, FILE *fout);

int cast_var(struct var var, var_type cast);
union var_val type_default_val(var_type type);

// arithmetic
struct var negate_var(struct var a);
struct var add_var(struct var a, struct var b);
struct var sub_var(struct var a, struct var b);
struct var mul_var(struct var a, struct var b);
struct var div_var(struct var a, struct var b);

#endif /* ifndef VAR_H */
