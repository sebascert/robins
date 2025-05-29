#include "frontend/yyshared.h"

#include <stdio.h>

char *yyfilename;

ast_evaluator eval_ast_statement;

void yyerror(const char *s)
{
    fprintf(stderr, "%s:%d: %s\n", yyfilename, yylineno, s);
}
