%{
#include <stdlib.h>
#include <stdio.h>
#include "parser.h"
#include "interpret.h"

int yylex();
%}

%union {
    char *id;
    var_type type;
    union var_val vval;
    struct ast_node* nptr;
}

/* keywords */
%token PRINT
%token INT
%token FLOAT
/* literals */
%token <vval> INT_L
%token <vval> FLOAT_L
/* identifiers */
%token <id> VAR
/* types */
%type <type> type
%type <nptr> declaration statement
%type <nptr> expression term factor num_const

%%
program:
    /* epsilon */               { exit(0); }
|   '\n' program
|   declaration '\n' program    { eval_ast($1); free_node($1); }
|   statement '\n' program      { eval_ast($1); free_node($1); }
|   error '\n'
;

declaration:
    type VAR                    { $$ = push_op($1, 1, $2); }
|   type VAR '=' expression     { $$ = push_op($1, 2, $2, $4); }
;

statement:
    expression                  { $$ = $1; }
|   PRINT expression '\n'       { $$ = push_op('p', 1, $2); }
|   VAR '=' expression '\n'     { $$ = push_op('-', 2, $1, $3); }
;

expression:
    term                        { $$ = $1; }
|   '-' term                    { $$ = push_op('-', 1, $2); }
|   term '+' expression         { $$ = push_op('+', 2, $1, $3); }
|   term '-' expression         { $$ = push_op('-', 2, $1, $3); }
;
term:
    factor                      { $$ = $1; }
|   factor '*' term             { $$ = push_op('*', 2, $1, $3); }
|   factor '/' term             { $$ = push_op('/', 2, $1, $3); }
;
factor:
    VAR                         { $$ = push_id($1); }
|   num_const                   { $$ = $1; }
|   '(' expression ')'          { $$ = $2; }
;

num_const:
    INT_L                       { $$ = push_const(VTYPE_INT, $1); }
|   FLOAT_L                     { $$ = push_const(VTYPE_FLOAT, $1); }
;

type:
    INT                         { $$ = VTYPE_INT; }
|   FLOAT                       { $$ = VTYPE_FLOAT; }
;
%%
