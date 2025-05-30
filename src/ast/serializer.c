#include "ast/serializer.h"

#include "ast/node.h"
#include "ast/nodes/argument.h"

#include <stdbool.h>
#include <stdio.h>

static size_t curr_id[] = {
    [ASTNODE_T_STATEMENT] = 0, [ASTNODE_T_INSTRUCTION] = 0,
    [ASTNODE_T_ARGUMENT] = 0,  [ASTNODE_T_OPERATION] = 0,
    [ASTNODE_T_LITERAL] = 0,
};

// returns the id of the serialized node
size_t serialize_ast_nodes(const struct astnode *node, FILE *fout)
{
    if (!node)
        return (size_t)-1;

    const char *label;
    size_t node_id;

    switch (node->type) {
        case ASTNODE_T_STATEMENT:
            node_id = curr_id[ASTNODE_T_STATEMENT]++;
            label = "stmt";
            for (size_t i = 0; i < node->stmt.n_ins; i++) {
                struct astnode *instruction = node->stmt.instructions[i];
                size_t instruction_node_id =
                    serialize_ast_nodes(instruction, fout);
                if (instruction_node_id == (size_t)-1)
                    goto invalid_ast;

                fprintf(fout, "_%s%lu -> _%s%lu\n",
                        astnode_t_names[ASTNODE_T_STATEMENT], node_id,
                        astnode_t_names[instruction->type],
                        instruction_node_id);
            }
            break;

        case ASTNODE_T_INSTRUCTION:
            node_id = curr_id[ASTNODE_T_INSTRUCTION]++;
            label = instruction_t_mnemonics[node->ins.type];
            for (size_t i = 0; i < node->ins.n_args; i++) {
                struct astnode *arg = node->ins.args[i];
                size_t arg_node_id = serialize_ast_nodes(arg, fout);
                if (arg_node_id == (size_t)-1)
                    goto invalid_ast;

                fprintf(fout, "_%s%lu -> _%s%lu\n",
                        astnode_t_names[ASTNODE_T_INSTRUCTION], node_id,
                        astnode_t_names[arg->type], arg_node_id);
            }
            break;

        case ASTNODE_T_ARGUMENT:
            node_id = curr_id[ASTNODE_T_ARGUMENT]++;
            label = argument_t_names[node->arg.type];

            struct astnode *resolution = node->arg.resolution;
            size_t resolution_node_id = serialize_ast_nodes(resolution, fout);
            if (resolution_node_id == (size_t)-1)
                goto invalid_ast;

            fprintf(fout, "_%s%lu -> _%s%lu\n",
                    astnode_t_names[ASTNODE_T_ARGUMENT], node_id,
                    astnode_t_names[resolution->type], resolution_node_id);
            break;

        case ASTNODE_T_OPERATION:
            node_id = curr_id[ASTNODE_T_OPERATION]++;
            label = operation_t_names[node->op.type];
            for (size_t i = 0; i < node->op.n_operands; i++) {
                struct astnode *operand = node->op.operands[i];
                size_t operand_node_id = serialize_ast_nodes(operand, fout);
                if (operand_node_id == (size_t)-1)
                    goto invalid_ast;

                fprintf(fout, "_%s%lu -> _%s%lu\n",
                        astnode_t_names[ASTNODE_T_OPERATION], node_id,
                        astnode_t_names[operand->type], operand_node_id);
            }
            break;

        case ASTNODE_T_LITERAL:
            node_id = curr_id[ASTNODE_T_LITERAL]++;
            fprintf(fout, "_%s%lu [label=\"",
                    astnode_t_names[ASTNODE_T_LITERAL], node_id);
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
    return (size_t)-1;
}

FILE *serialize_ast_out;
void serialize_ast(const struct astnode *ast)
{
    for (size_t i = 0; i < (sizeof(curr_id) / sizeof(size_t)); i++) {
        curr_id[i] = 0;
    }

    fprintf(serialize_ast_out, "digraph Tree {\n");

    if (serialize_ast_nodes(ast, serialize_ast_out) == (size_t)-1) {
        fprintf(stderr, "attempting to serialize invalid AST\n");
        return;
    }

    fprintf(serialize_ast_out, "}\n");
}
