#include <stdio.h>

#include "interpret.h"
#include "parse.h"

ast_evaluator eval_ast;

int yyparse(void);

int main(void) {
    fprintf(stderr, "simple calc program\n");

    eval_ast = execute;

    int yycode = yyparse();

    return yycode;
}
