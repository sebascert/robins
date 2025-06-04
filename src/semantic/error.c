#include "semantic/error.h"

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
