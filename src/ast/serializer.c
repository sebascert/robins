#include "ast/serializer.h"

#include "ast/literal.h"
#include "ast/node.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

static size_t curr_id[] = {
    [ASTNODE_T_OPERATION] = 0,
    [ASTNODE_T_LITERAL] = 0,
};

// returns the id of the serialized node
size_t serialize_ast_nodes(const struct astnode *node, FILE *fout)
{
    if (!node)
        return -1;

    const char *label;
    size_t node_id;

    switch (node->type) {
        case ASTNODE_T_OPERATION:
            node_id = curr_id[ASTNODE_T_OPERATION]++;
            label = operation_t_names[node->op.type];
            for (size_t i = 0; i < node->op.n_operands; i++) {
                struct astnode *operand = node->op.operands[i];
                size_t operand_node_id = serialize_ast_nodes(operand, fout);
                if (errno)
                    goto invalid_ast;

                fprintf(fout, "_%s%lu -> _%s%lu\n",
                        astnode_t_names[ASTNODE_T_OPERATION], node_id,
                        astnode_t_names[operand->type], operand_node_id);
            }
            break;

        case ASTNODE_T_LITERAL:
            node_id = curr_id[ASTNODE_T_LITERAL]++;
            fprintf(fout, "_%s%lu [label=\"",
                    astnode_t_names[ASTNODE_T_OPERATION], node_id);
            print_lit(&node->lit, fout);
            fprintf(fout, "\"]\n");
            return node_id;

        default:
            goto invalid_ast;
    }

    fprintf(fout, "_%s%lu [label=\"%s\"]\n", astnode_t_names[node->type],
            node_id, label);
    return node_id;
invalid_ast:
    errno = 1;
    return -1;
}

FILE *serialize_ast_out;
void serialize_ast(const struct astnode *ast)
{
    for (size_t i = 0; i < (sizeof(curr_id) / sizeof(size_t)); i++) {
        curr_id[i] = 0;
    }

    fprintf(serialize_ast_out, "digraph Tree {\n");

    serialize_ast_nodes(ast, serialize_ast_out);
    if (errno) {
        perror("attempting to serialize invalid AST\n");
        return;
    }

    fprintf(serialize_ast_out, "}\n");
}
