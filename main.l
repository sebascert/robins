%{
#include <stdio.h>
%}

%%
[a-zA-Z][a-zA-Z0-9]*    { printf("WORD "); }
[a-zA-Z0-9\/.-]+        { printf("FILENAME "); }
\"                      { printf("QUOTE "); }
\{                      { printf("OBRACE "); }
\}                      { printf("EBRACE "); }
;                       { printf("SEMICOLON "); }
\n                      { printf("\n"); }
[ \t]+                  /* ignore whitespace */
%%

int main(int argc, char** argv) {
    if (argc != 2){
        return 1;
    }

    FILE* fd = fopen(argv[1], "r");
    if (!fd){
        return 1;
    }

    yyset_in(fd);
    yylex();
    fclose(fd);

    return 0;
}
