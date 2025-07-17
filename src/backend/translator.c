#include "backend/translator.h"

#include "ast/node.h"
#include "backend/expr_eval.h"
#include "semantic/validator.h"

#include <stdio.h>

#define INVALID_AST 1
#define SEMANTIC_ERROR 2

int translate_ast_node(const struct astnode *node, FILE *fout)
{
    if (!node)
        return INVALID_AST;

    switch (node->type) {
        case ASTNODE_T_STATEMENT:
            for (size_t i = 0; i < node->stmt.n_ins; i++) {
                struct astnode *instruction = node->stmt.instructions[i];
                int status = translate_ast_node(instruction, fout);
                if (status != 0)
                    return status;
            }
            break;

        case ASTNODE_T_INSTRUCTION:
            fprintf(fout, "%s", instruction_t_mnemonics[node->ins.type]);
            for (size_t i = 0; i < node->ins.n_args; i++) {
                int status = translate_ast_node(node->ins.args[i], fout);
                if (status != 0)
                    return status;
            }
            fprintf(fout, "\n");
            break;

        case ASTNODE_T_ARGUMENT: {
            struct literal evaluated = expr_eval(node->arg.resolution);
            if (evaluated.type == LITERAL_T_NULL)
                return INVALID_AST;

            if (!arg_semantic_validate(node->arg.type, &evaluated)) {
                fprintf(stderr, "Halted translation due to semantic error\n");
                return SEMANTIC_ERROR;
            }

            fprintf(fout, " ");
            print_lit(&evaluated, fout);
            break;
        }

        case ASTNODE_T_OPERATION:;
        case ASTNODE_T_LITERAL:
        default:
            return INVALID_AST;
    }
    return 0;
}

FILE *translate_ast_out;
void translate_ast(const struct astnode *ast)
{
    if (translate_ast_node(ast, translate_ast_out) == INVALID_AST) {
        fprintf(stderr, "attempting to translate invalid AST\n");
    }
}
