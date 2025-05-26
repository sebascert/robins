#include "interpret.h"
#include "parse.h"

static int serialized_ntype_count[] = {
    [NTYPE_OP] = 0,
    [NTYPE_CONST] = 0,
};

const char *serialize_op(int operand) {
    switch (operand) {
    case '-':
        return "-";
    case '+':
        return "+";
    case '*':
        return "*";
    case '/':
        return "/";
    default:
        sserror(true, "operand_to_str: invalid operand\n");
        return NULL;
    }
}

int serialize_ast_nodes(const struct ast_node *node, FILE *fout) {
    if (!node)
        return -1;
    static const char *ntype_strid[] = {
        [NTYPE_OP] = "op",
        [NTYPE_CONST] = "cst",
    };

    const char *label;
    int type = node->type;
    size_t node_id;

    switch (type) {
    case NTYPE_OP:
    case NTYPE_CONST:
        node_id = serialized_ntype_count[type]++;
        break;

    default:
        sserror(true, "serialize_ast_nodes: invalid node type\n");
        return -1;
    }

    switch (type) {
    case NTYPE_OP:
        label = serialize_op(node->op.op);
        for (size_t i = 0; i < node->op.n_operands; i++) {
            struct ast_node *operand = node->op.operands[i];
            int operand_node_id = serialize_ast_nodes(operand, fout);
            if (operand_node_id == -1) {
                sserror(false,
                        "serialize_ast_nodes: null operand in op_node\n");
                continue;
            }
            fprintf(fout, "_op%lu -> _%s%lu\n", node_id,
                    ntype_strid[operand->type], (size_t)operand_node_id);
        }
        break;

    case NTYPE_CONST:
        fprintf(fout, "_cst%lu [label=\"", node_id);
        switch (node->cst.v.type) {
        case VTYPE_INT:
            fprintf(fout, "%d", node->cst.v.val.ival);
            break;

        case VTYPE_FLOAT:
            fprintf(fout, "%f", node->cst.v.val.fval);
            break;

        default:
            sserror(true, "serialize_ast_nodes: invalid cst_node vtype\n");
            return -1;
        }
        fprintf(fout, "\"]\n");
        return node_id;

    default:
        sserror(true, "serialize_ast_nodes: invalid node type\n");
        return -1;
    }

    fprintf(fout, "_%s%lu [label=\"%s\"]\n", ntype_strid[type], node_id, label);
    return node_id;
}

void serialize_ast(const struct ast_node *ast, FILE *fout) {
    for (size_t i = 0; i < (sizeof(serialized_ntype_count) / sizeof(int));
         i++) {
        serialized_ntype_count[i] = 0;
    }

    fprintf(fout, "digraph Tree {\n");

    serialize_ast_nodes(ast, fout);

    fprintf(fout, "}\n");
}
