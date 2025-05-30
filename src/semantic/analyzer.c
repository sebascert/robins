#include "semantic/analyzer.h"

#include "backend/arithmetic.h"
#include "frontend/yyshared.h"

#include <stdarg.h>
#include <stdio.h>

void sserror(const char* s, ...)
{
    if (!s)
        s = "semantic error";

    va_list args;
    va_start(args, s);

    fprintf(stderr, "%s:%d: ", yyfilename, yylineno);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");

    va_end(args);
}

const literal_t arg_types[] = {
    [ARG_DEGREES] = LITERAL_T_INT, /* macroized */
    [ARG_BLOCKS] = LITERAL_T_INT,  /* macroized */
};

bool arg_semantic_validate(argument_t type, struct literal* evaluated_arg)
{
    if (cast_lit(evaluated_arg, arg_types[type]) != 0) {
        sserror("invalid type '%s' for %s",
                literal_t_names[evaluated_arg->type], argument_t_names[type]);
        return false;
    }

    switch (type) {
        case ARG_DEGREES: /* macroized */
            return arg_sv_degrees(evaluated_arg->val.ival);
        case ARG_BLOCKS: /* macroized */
            return arg_sv_blocks(evaluated_arg->val.ival);
    }

    return false;
}

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
