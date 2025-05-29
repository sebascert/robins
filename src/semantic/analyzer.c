#include "semantic/analyzer.h"

#include "frontend/yyshared.h"

#include <stdarg.h>
#include <stdio.h>

void sserror(const char *s, ...)
{
    va_list args;

    va_start(args, s);
    fprintf(stderr, "%s:%d: %s\n", yyfilename, yylineno, s);
    if (s)
        vfprintf(stderr, s, args);
    va_end(args);
}

const literal_t arg_types[] = {
    [ARG_DEGREES] = LITERAL_T_INT, /* macroized */
};

bool arg_semantic_validate(argument_t type, struct literal evaluated_arg)
{
    if (arg_types[type] != evaluated_arg.type) {
        sserror("invalid type '%s' for %s", literal_t_names[evaluated_arg.type],
                argument_t_names[type]);
        return false;
    }

    switch (type) {
        case ARG_DEGREES: /* macroized */
            return arg_sv_degrees(evaluated_arg.val.ival);
    }

    return false;
}

/* argument semantic validators */
bool arg_sv_degrees(int value) /* macroized */
{
    (void)value;
    return true;
}
