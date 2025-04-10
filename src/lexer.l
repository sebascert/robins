%{
#include <stdlib.h>
#include "y.tab.h"
%}

num         [0-9]
alpha       [A-Za-z]
alphanum    [A-Za-z0-9]
_alpha      [_A-Za-z]
_alphanum   [_A-Za-z0-9]

%%
    /* keywords */
i                       { return INT; }
f                       { return FLOAT; }
p                       { return PRINT; }
    /* operators */
\+                      { return *yytext; }
-                       { return *yytext; }
\*                      { return *yytext; }
\/                      { return *yytext; }
    /* literals */
-?{num}+                  { yylval = atoi(yytext);  return INT_L; }
-?\.{num}+                { yylval = strtof(yytext, NULL);  return FLOAT_L; }
-?{num}+\.{num}*          { yylval = strtof(yytext, NULL);  return FLOAT_L; }
    /* identifiers */
{_alpha}{_alphanum}*  { yylval = yytext; return IDENTIFIER; }
    /* ignores */
\/\/.*                  ; /* ignore comments */
[ \t]                   ; /* ignore whitespace */
    /* keywords */
\n                      { return *yytext; }
%%
