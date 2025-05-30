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
"Robot"                         { return NOUN; } /* macroized */
"robot"                         { return NOUN; } /* macroized */
    /* statement polite words */
"please"                        { return POLITE_WORDS; } /* macroized */
"would you please"              { return POLITE_WORDS; } /* macroized */
    /* instruction conjunction */
", and then"                    { return INS_CONJUNCTION; } /* macroized */
", then"                        { return INS_CONJUNCTION; } /* macroized */
    /* instruction verbs */
"rotate"                        { return INS_ROTATE_VERB; } /* macroized */
"move"                          { return INS_MOVE_VERB; } /* macroized */
    /* argument units */
"deg"                           { return ARG_DEGREES_UNIT; } /* macroized */
"blocks"                        { return ARG_BLOCKS_UNIT; } /* macroized */
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
