%{
#include <stdio.h>
int yylex();
void yyerror(const char *s);
%}


%token NAME NUMBER
%%
statement: NAME '=' expression { printf("%c = %d\n", $1, $3); }
 | expression { printf("= %d\n", $1); }
 ;
expression: term { $$ = $1; }
 | expression '+' term { $$ = $1 + $3; }
 | expression '-' term { $$ = $1 - $3; }
 ;
term: factor { $$ = $1; }
 | term '*' factor { $$ = $1 * $3; }
 ;
factor: NUMBER { $$ = $1; }
 | '(' expression ')' { $$ = $2; }
 ;
%%
