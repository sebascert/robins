#ifndef INTERPRET_H
#define INTERPRET_H

#include "ast/node.h"

#include <stdbool.h>
#include <stdio.h>

void initialize_interpreter(void);

// output file of interpret_ast
extern FILE *interpret_ast_out;

// interpret abstract syntax tree
void interpret_ast(const struct astnode *ast);

#endif /* ifndef INTERPRET_H */
