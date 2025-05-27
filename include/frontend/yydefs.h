#ifndef FRONTEND_YYDEFS_H
#define FRONTEND_YYDEFS_H

#include <stdio.h>

#include "ast/node.h"

extern int yylineno;
extern char *yyfilename;
extern FILE *yyin;

typedef void (*ast_evaluator)(const struct ast_node *);
extern ast_evaluator eval_ast_statement;

extern ast_evaluator eval_ast_statement;

int yylex();

#endif /* ifndef FRONTEND_YYDEFS_H */
