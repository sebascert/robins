#include <stdio.h>

#include "interpret.h"
#include "parse.h"

static FILE *fout;

ast_evaluator eval_ast_statement;
void out_graph(const struct ast_node *ast) { serialize_ast(ast, fout); }
void interpret(const struct ast_node *ast) { interpret_ast(ast, fout); }

int yyparse(void);

int main(int arc, char **argv) {
    fprintf(stderr, "simple calc program\n");

    fout = stdout;
    eval_ast_statement = interpret;

    int yycode = yyparse();

    return yycode;
}
