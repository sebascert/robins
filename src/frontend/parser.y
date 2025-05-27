%{
#include "ast/node.h"
#include "frontend/yyshared.h"

#include <stdlib.h>
#include <stdio.h>
%}

%union {
    union literal_v lval;
    struct astnode* nptr;
}

/* literals */
%token <lval> INT_L
%token <lval> REAL_L
/* types */
%type <nptr> num_literal
%type <nptr> statement
%type <nptr> num_expression term factor

%%
program:
    /* epsilon */               { exit(0); }
|   '\n' program
|   error '\n' program          { yyerrok; }
|   statement '\n' program
;

statement:
    num_expression              { eval_ast_statement($1); free_node($1); }
;

num_expression:
    term                        { $$ = $1; }
|   '-' term                    { $$ = push_op('-', 1, $2); }
|   num_expression '+' term     { $$ = push_op('+', 2, $1, $3); }
|   num_expression '-' term     { $$ = push_op('-', 2, $1, $3); }
;
term:
    factor                      { $$ = $1; }
|   term '*' factor             { $$ = push_op('*', 2, $1, $3); }
|   term '/' factor             { $$ = push_op('/', 2, $1, $3); }
;
factor:
    num_literal                 { $$ = $1; }
|   '(' num_expression ')'      { $$ = $2; }
;

num_literal:
    INT_L                       { $$ = push_literal(LITERAL_T_INT, $1); }
|   REAL_L                     { $$ = push_literal(LITERAL_T_REAL, $1); }
;
%%
