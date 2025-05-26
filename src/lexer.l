%{
#include <stdlib.h>
#include "parse.h"
#include "y.tab.h"
%}

num         [0-9]
alpha       [A-Za-z]
alphanum    [A-Za-z0-9]
_alpha      [_A-Za-z]
_alphanum   [_A-Za-z0-9]

%%
    /* operators */
[-+*/()]                       { return *yytext; }
    /* literals */
{num}+                          { yylval.vval.ival = atoi(yytext);  return INT_L; }
(\.{num}+|{num}+\.{num}*)       { yylval.vval.fval = strtof(yytext, NULL);  return FLOAT_L; }
    /* ignores */
\/\/.*                          ; /* ignore comments */
[ \t]                           ; /* ignore whitespace */
    /* statement terminator */
\n                              { return *yytext; }
    /* lexical error */
.                               { yyerror("invalid token"); }
%%
