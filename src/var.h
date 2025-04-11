#ifndef VAR_H
#define VAR_H

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

static struct var NULL_VAR = {
    .type = VTYPE_NULL,
    .val = {0},
};

#endif /* ifndef VAR_H */
