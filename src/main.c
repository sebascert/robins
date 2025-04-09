#include <stdio.h>

int yyparse(void);

int main(void) {
    fprintf(stderr, "simple calc program\n");
    return yyparse();
}
