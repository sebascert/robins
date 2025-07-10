include(`config.m4')dnl
%{
#include "ast/node.h"
#include "frontend/yyshared.h"
#include "parser.h"

static char lex_err[] = "error: unexpected token ' '";
%}

num         [0-9]
alpha       [A-Za-z]
alphanum    [A-Za-z0-9]
_alpha      [_A-Za-z]
_alphanum   [_A-Za-z0-9]

%%
 /* INSTRUCTION LEXEMAS */
    /* robot noun (statement prefix) */
ROBINS_NOUN_LEXEMAS()
    /* statement polite words */
ROBINS_POLITE_WORDS_LEXEMAS()
    /* instruction conjunction (may get macroized later) */
ROBINS_CONJUNCTION_LEXEMAS()
    /* instruction verbs */
ROBINS_INS_VERB_LEXEMAS()
    /* argument units */
ROBINS_ARG_UNIT_LEXEMAS()
    /* statement terminator */
\.                              { return '.'; }

 /* ARITHMETIC LEXEMAS */
    /* operators */
[-+*/()]                        { return *yytext; }
    /* literals */
{num}+                          { yylval.lval.ival = atoi(yytext);  return INT_L; }
(\.{num}+|{num}+\.{num}*)       { yylval.lval.rval = strtof(yytext, NULL);  return REAL_L; }

 /* LEXICAL IGNORES */
    /* ignore comments */
\/\/.*                          { yylineno++; }
    /* ignore whitespace */
\n                              { yylineno++; }
[ \t]                         ;

 /* LEXICAL ERROR */
.  {
    lex_err[25] = *yytext;
    yyerror(lex_err);

    int c;
    while ((c = input()) != '\n' && c != EOF) ;
    yylineno++;
    return '.';
}
%%
