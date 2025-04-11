#include "interpret.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct var_entry {
    char id[VAR_ID_SIZE];
    var_type type;
    union var_val val;
};
struct var_entry variables[MAX_VAR_ENTRIES];

const char *const vtype_names[] = {
    [VTYPE_INT] = "int",
    [VTYPE_FLOAT] = "float",
};

// utils
int decl_var(const char *id, var_type type, union var_val val);
int decl_or_set_var(const char *id, var_type type, union var_val val);

struct var_entry *get_var(const char *id);
int set_var(const char *id, var_type type, union var_val val);

bool valid_id(const char *id);
bool compatible_types(var_type dest, var_type orig);

union var_val default_var_val(var_type type);

void initialize_interpreter(void) {
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        memset(variables[i].id, 0, VAR_ID_SIZE);
    }
}

void interpret_ast_node(const struct ast_node *node, FILE *fout) {
    if (!node) {
        return;
    }
}

void interpret_ast(const struct ast_node *ast, FILE *fout) {
    if (!ast) {
        sserror(true, "interpret_ast: null ast");
        return;
    }

    interpret_ast_node(ast, fout);
}

static int serialized_ntype_count[] = {
    [NTYPE_ID] = 0,
    [NTYPE_OP] = 0,
    [NTYPE_CONST] = 0,
};

int serialize_ast_nodes(const struct ast_node *node, FILE *fout) {
    if (!node)
        return -1;
    static const char *ntype_strid[] = {
        [NTYPE_ID] = "id",
        [NTYPE_OP] = "op",
        [NTYPE_CONST] = "cst",
    };

    const char *label;
    int type = node->type;
    size_t node_id;

    switch (type) {
    case NTYPE_ID:
    case NTYPE_OP:
    case NTYPE_CONST:
        node_id = serialized_ntype_count[type]++;
        break;

    default:
        sserror(true, "serialize_ast_nodes: invalid node type\n");
        return -1;
    }

    switch (type) {
    case NTYPE_ID:
        label = node->id.id;
        break;

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
        switch (node->cst.type) {
        case VTYPE_INT:
            fprintf(fout, "%d", node->cst.val.ival);
            break;

        case VTYPE_FLOAT:
            fprintf(fout, "%f", node->cst.val.fval);
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

int decl_var(const char *id, var_type type, union var_val val) {
    if (!valid_id(id))
        return 1;

    struct var_entry *free_entry = NULL;
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        if (strcmp(variables[i].id, id) == 0) {
            sserror(false, "decl_var: redeclaring '%s'\n", id);
            return 1;
        }

        if (!free_entry && *variables[i].id == '\0')
            free_entry = &variables[i];
    }

    if (!free_entry) {
        sserror(false, "decl_var: max variables stored reached\n");
        return 1;
    }

    snprintf(free_entry->id, VAR_ID_SIZE, "%s", id);
    free_entry->val = val;
    free_entry->type = type;
    return 0;
}

int decl_or_set_var(const char *id, var_type type, union var_val val) {
    if (!valid_id(id))
        return 1;

    struct var_entry *free_entry = NULL;
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        if (strcmp(variables[i].id, id) == 0)
            return set_var(id, type, val);

        if (!free_entry && *variables[i].id == '\0')
            free_entry = &variables[i];
    }

    if (!free_entry) {
        sserror(false, "decl_or_set_var: max variables stored reached\n");
        return 1;
    }

    snprintf(free_entry->id, VAR_ID_SIZE, "%s", id);
    free_entry->val = val;
    free_entry->type = type;
    return 0;
}

struct var_entry *get_var(const char *id) {
    if (!valid_id(id))
        return NULL;

    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        if (strcmp(variables[i].id, id) == 0)
            return &variables[i];
    }

    return NULL;
}

int set_var(const char *id, var_type type, union var_val val) {
    struct var_entry *var;

    if (!(var = get_var(id)))
        return 1;

    if (compatible_types(var->type, type)) {
        var->val = val;
        return 0;
    }

    sserror(false, "set_var: assigning %s to variable of type %s\n",
            variables[type], variables[var->type]);
    return 1;
}

bool valid_id(const char *id) {
    if (!id) {
        sserror(true, "valid_id: null id string\n");
        return false;
    }

    if (strlen(id) >= VAR_ID_SIZE) {
        sserror(false, "valid_id: id '%s' is too long\n", id);
        return false;
    }

    return true;
}

bool compatible_types(var_type dest, var_type orig) {
    if (dest == orig)
        return true;

    switch (dest) {
    case VTYPE_INT:
        return false;

    case VTYPE_FLOAT:
        if (orig == VTYPE_INT)
            return true;
        return false;

    default:
        return false;
    }
}

union var_val default_var_val(var_type type) {
    switch (type) {
    case VTYPE_INT:
        return (union var_val)0;

    case VTYPE_FLOAT:
        return (union var_val)0;

    default:
        sserror(true,
                "default_var_val: unknown type '%s' has no default value\n",
                vtype_names[type]);
        return (union var_val)0;
    }
}

void sserror(bool terminate, const char *s, ...) {
    va_list args;

    va_start(args, s);
    fprintf(stderr, "semantic error: ");
    if (s)
        vfprintf(stderr, s, args);
    va_end(args);

    if (terminate)
        exit(1);
}
