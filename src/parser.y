%{
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"
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
%type <nptr> id num_const
%type <nptr> primary_statement statement declaration
%type <nptr> expression term factor

%%
program:
    /* epsilon */               { exit(0); }
|   '\n' program
|   primary_statement '\n' program
|   error '\n'
;

primary_statement:
    statement                   { eval_ast_statement($1); free_node($1); }
|   declaration                 { eval_ast_statement($1); free_node($1); }
;

declaration:
    type id                     { $$ = push_op($1, 1, $2); }
|   type id '=' expression      { $$ = push_op($1, 2, $2, $4); }
;

statement:
    expression                  { $$ = $1; }
|   PRINT expression            { $$ = push_op('p', 1, $2); }
|   id '=' expression           { $$ = push_op('=', 2, $1, $3); }
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
    id                          { $$ = $1; }
|   num_const                   { $$ = $1; }
|   '(' expression ')'          { $$ = $2; }
;

id:
    VAR                         { $$ = push_id($1); }
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
