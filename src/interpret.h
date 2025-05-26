#ifndef INTERPRET_H
#define INTERPRET_H

#include "parse.h"

#include <stdbool.h>
#include <stdio.h>

void initialize_interpreter(void);

// serialize ast into graphviz representation
void serialize_ast(const struct ast_node *ast, FILE *fout);

// interpret abstract syntax tree
void interpret_ast(const struct ast_node *ast, FILE *fout);

void sserror(bool terminate, const char *s, ...);

#endif /* ifndef INTERPRET_H */
