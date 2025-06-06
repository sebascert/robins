#include "backend/arithmetic.h"

int cast_lit(struct literal *lit, literal_t cast)
{
    if (lit->type == cast)
        return 0;

    switch (lit->type) {
        case LITERAL_T_INT:
            if (cast == LITERAL_T_REAL) {
                lit->val.rval = (literal_t_real)lit->val.ival;
                goto casted;
            }
            return 1;

        case LITERAL_T_REAL:
            if (cast == LITERAL_T_INT) {
                lit->val.ival = (literal_t_int)lit->val.rval;
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

union literal_v literal_default(literal_t type)
{
    switch (type) {
        case LITERAL_T_INT:
            return (union literal_v){.ival = 0};

        case LITERAL_T_REAL:
            return (union literal_v){.rval = 0.0f};

        default:
            return (union literal_v)0;
    }
}

struct literal negate_lit(struct literal a)
{
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

struct literal add_lit(struct literal a, struct literal b)
{
    if (a.type == LITERAL_T_REAL || b.type == LITERAL_T_REAL) {
        literal_t_real lhs =
            a.type == LITERAL_T_REAL ? a.val.rval : (literal_t_real)a.val.ival;
        literal_t_real rhs =
            b.type == LITERAL_T_REAL ? b.val.rval : (literal_t_real)b.val.ival;
        return (struct literal){LITERAL_T_REAL, .val.rval = lhs + rhs};
    } else {
        return (struct literal){LITERAL_T_INT,
                                .val.ival = a.val.ival + b.val.ival};
    }
}

struct literal sub_lit(struct literal a, struct literal b)
{
    if (a.type == LITERAL_T_REAL || b.type == LITERAL_T_REAL) {
        literal_t_real lhs =
            a.type == LITERAL_T_REAL ? a.val.rval : (literal_t_real)a.val.ival;
        literal_t_real rhs =
            b.type == LITERAL_T_REAL ? b.val.rval : (literal_t_real)b.val.ival;
        return (struct literal){LITERAL_T_REAL, .val.rval = lhs - rhs};
    } else {
        return (struct literal){LITERAL_T_INT,
                                .val.ival = a.val.ival - b.val.ival};
    }
}

struct literal mul_lit(struct literal a, struct literal b)
{
    if (a.type == LITERAL_T_REAL || b.type == LITERAL_T_REAL) {
        literal_t_real lhs =
            a.type == LITERAL_T_REAL ? a.val.rval : (literal_t_real)a.val.ival;
        literal_t_real rhs =
            b.type == LITERAL_T_REAL ? b.val.rval : (literal_t_real)b.val.ival;
        return (struct literal){LITERAL_T_REAL, .val.rval = lhs * rhs};
    } else {
        return (struct literal){LITERAL_T_INT,
                                .val.ival = a.val.ival * b.val.ival};
    }
}

struct literal div_lit(struct literal a, struct literal b)
{
    literal_t_real lhs =
        a.type == LITERAL_T_REAL ? a.val.rval : (literal_t_real)a.val.ival;
    literal_t_real rhs =
        b.type == LITERAL_T_REAL ? b.val.rval : (literal_t_real)b.val.ival;

    if (rhs == 0)
        return NULL_LITERAL;
    return (struct literal){LITERAL_T_REAL, .val.rval = lhs / rhs};
}
