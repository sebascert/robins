%{
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
#include "interpret.h"

int yylex();
%}

%union {
    union var_val vval;
    struct ast_node* nptr;
}

/* literals */
%token <vval> INT_L
%token <vval> FLOAT_L
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
    INT_L                       { $$ = push_const(VTYPE_INT, $1); }
|   FLOAT_L                     { $$ = push_const(VTYPE_FLOAT, $1); }
;
%%
