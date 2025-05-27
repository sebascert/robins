%{
#include "ast/node.h"
#include "parser.h"

static char lex_err[] = "error: unexpected token ' '";
%}

num         [0-9]
alpha       [A-Za-z]
alphanum    [A-Za-z0-9]
_alpha      [_A-Za-z]
_alphanum   [_A-Za-z0-9]

%%
    /* operators */
[-+*/()]                        { return *yytext; }
    /* literals */
{num}+                          { yylval.vval.ival = atoi(yytext);  return INT_L; }
(\.{num}+|{num}+\.{num}*)       { yylval.vval.fval = strtof(yytext, NULL);  return FLOAT_L; }
    /* ignores */
\/\/.*                          ; /* ignore comments */
[ \t]                           ; /* ignore whitespace */
    /* statement terminator */
\n                              { yylineno++; return '\n'; }
    /* lexical error */
.  {
    lex_err[25] = *yytext;
    yyerror(lex_err);

    int c;
    while ((c = input()) != '\n' && c != EOF) ;
    yylineno++;
    return '\n';
}
%%
