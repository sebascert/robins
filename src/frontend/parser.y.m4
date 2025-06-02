include(`config.m4')dnl
ifdef(`ROBINS_INS_GRAMMAR_MACROS', , `errprint(`m4: ERROR: ROBINS_INS_GRAMMAR_MACROS is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_ARG_GRAMMAR_MACROS', , `errprint(`m4: ERROR: ROBINS_ARG_GRAMMAR_MACROS is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_INS_GRAMMAR_RULE_STRUCTURE', , `errprint(`m4: ERROR: ROBINS_INS_GRAMMAR_RULE_STRUCTURE is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_INS_PARTIAL_GRAMMAR_RULE', , `errprint(`m4: ERROR: ROBINS_INS_PARTIAL_GRAMMAR_RULE is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_INS_GRAMMAR_RULES', , `errprint(`m4: ERROR: ROBINS_INS_GRAMMAR_RULES is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_ARG_GRAMMAR_RULES', , `errprint(`m4: ERROR: ROBINS_ARG_GRAMMAR_RULES is not defined.')
m4exit(1)')dnl
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
%type <nptr> stmt ins partial_ins
/* instruction tokens and type decl */
ROBINS_INS_GRAMMAR_MACROS()
/* argument tokens and type decl */
ROBINS_ARG_GRAMMAR_MACROS()

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
ins:
ROBINS_INS_GRAMMAR_RULE_STRUCTURE()
;

partial_ins:
ROBINS_INS_PARTIAL_GRAMMAR_RULE()
;

ROBINS_INS_GRAMMAR_RULES()

/* arguments */
ROBINS_ARG_GRAMMAR_RULES()

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
