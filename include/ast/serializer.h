#ifndef AST_SERIALIZER_H
#define AST_SERIALIZER_H

#include "ast/node.h"

#include <stdio.h>

// output file of serialize_ast
extern FILE *serialize_ast_out;

// serialize ast into graphviz representation
void serialize_ast(const struct astnode *ast);

#endif /* ifndef AST_SERIALIZER_H */
