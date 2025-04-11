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
int print_var(struct var var, FILE *fout);

struct var var_negate(struct var a);
struct var var_add(struct var a, struct var b);
struct var var_sub(struct var a, struct var b);
struct var var_mul(struct var a, struct var b);
struct var var_div(struct var a, struct var b);

#endif /* ifndef VAR_H */
