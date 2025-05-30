#ifndef BACKEND_TRANSLATOR_H
#define BACKEND_TRANSLATOR_H

#include "ast/node.h"

#include <stdbool.h>
#include <stdio.h>

// output file of translate_ast
extern FILE *translate_ast_out;

// translate AST to target
void translate_ast(const struct astnode *ast);

#endif /* ifndef BACKEND_TRANSLATOR_H */
