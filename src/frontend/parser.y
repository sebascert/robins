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

/* STATEMENT */
/* full_stmt structural lexemas */
%token NOUN
%token POLITE_WORDS
%token INS_CONJUNCTION

/* INSTRUCTION */
%type <nptr> stmt ins any_ins
/* instruction tokens and type decl */
%token INS_ROTATE_VERB /* macroized */
%type <nptr> ins_rotate /* macroized */
/* argument tokens and type decl */
%token ARG_DEGREES_UNIT /* macroized */
%type <nptr> arg_degrees /* macroized */

/* ARITHMETIC */
/* literals */
%token <lval> INT_L
%token <lval> REAL_L
/* types */
%type <nptr> num_literal
%type <nptr> num_expr term factor

%%
/* full_stmt */
statement_list:
    /* epsilon */               { exit(0); }
|   '.' statement_list
|   error '.' statement_list    { yyerrok; }
|   full_stmt statement_list
;

full_stmt:
    NOUN stmt '.'               { eval_ast_statement($2); free_node($2); }
;

stmt:
    ins                         { $$ = push_stmt($1); }
|   stmt INS_CONJUNCTION ins    { $$ = stmt_append_ins($1, $3); }
;

/* instructions */
ins: /* unpolite, polite, or both modes could be supported */
    any_ins                     { $$ = $1; } /* macroized */
|   POLITE_WORDS any_ins        { $$ = $2; } /* macroized */
;

any_ins:
    ins_rotate                  { $$ = $1; } /* macroized */
;

ins_rotate: /* macroized */
    INS_ROTATE_VERB arg_degrees { $$ = push_ins(INS_ROTATE, 1, $2); }
;

/* arguments */
arg_degrees: /* macroized */
    num_expr ARG_DEGREES_UNIT   { $$ = push_arg(ARG_DEGREES, $1); }
;

/* arithmetic */
num_expr:
    term                        { $$ = $1; }
|   '-' term                    { $$ = push_op('-', 1, $2); }
|   num_expr '+' term           { $$ = push_op('+', 2, $1, $3); }
|   num_expr '-' term           { $$ = push_op('-', 2, $1, $3); }
;
term:
    factor                      { $$ = $1; }
|   term '*' factor             { $$ = push_op('*', 2, $1, $3); }
|   term '/' factor             { $$ = push_op('/', 2, $1, $3); }
;
factor:
    num_literal                 { $$ = $1; }
|   '(' num_expr ')'            { $$ = $2; }
;

num_literal:
    INT_L                       { $$ = push_literal(LITERAL_T_INT, $1); }
|   REAL_L                      { $$ = push_literal(LITERAL_T_REAL, $1); }
;
%%
