#include "semantic/argument.h"

const literal_t arg_types[] = {
    [ARG_DEGREES] = LITERAL_T_INT, /* macroized */
    [ARG_BLOCKS] = LITERAL_T_INT,  /* macroized */
};

/* argument semantic validators */
bool arg_sv_degrees(int value) /* macroized */
{
    (void)value;
    return true;
}

bool arg_sv_blocks(int value) /* macroized */
{
    (void)value;
    return true;
}

bool user_arg_sv(argument_t type, struct literal* evaluated_arg)
{
    switch (type) {
        case ARG_DEGREES: /* macroized */
            return arg_sv_degrees(evaluated_arg->val.ival);
        case ARG_BLOCKS: /* macroized */
            return arg_sv_blocks(evaluated_arg->val.ival);
    }

    return false;
}
