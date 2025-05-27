#include "semantic/analyzer.h"
#include "frontend/yyshared.h"

#include <stdarg.h>
#include <stdio.h>

void sserror(const char *s, ...) {
    va_list args;

    va_start(args, s);
    fprintf(stderr, "%s:%d:  %s\n", yyfilename, yylineno, s);
    if (s)
        vfprintf(stderr, s, args);
    va_end(args);
}
