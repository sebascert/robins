#ifndef INTERPRET_H
#define INTERPRET_H

#include "parse.h"

#include <stdbool.h>
#include <stdio.h>

#define VAR_ID_SIZE 10
#define MAX_VAR_ENTRIES 26

void initialize_interpreter(void);

// serialize ast into graphviz representation
void serialize_ast(const struct ast_node *ast, FILE *fout);

void execute(const struct ast_node *node);

void sserror(bool terminate, const char *s, ...);

#endif /* ifndef INTERPRET_H */
