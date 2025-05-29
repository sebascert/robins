#ifndef FRONTEND_YYSHARED_H
#define FRONTEND_YYSHARED_H

#include "ast/node.h"

#include <stdio.h>

extern int yylineno;
extern char *yyfilename;
extern FILE *yyin;

typedef void (*ast_evaluator)(const struct astnode *);
extern ast_evaluator eval_ast_statement;

int yylex();
int yyparse(void);

void yyerror(const char *s);

#endif /* ifndef FRONTEND_YYSHARED_H */
