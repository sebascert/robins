#include "interpret.h"
#include "parse.h"
#include "var.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// var entry
struct var_entry {
    char id[VAR_ID_SIZE];
    struct var v;
};

struct var_entry variables[MAX_VAR_ENTRIES];

const char *const vtype_names[] = {
    [VTYPE_INT] = "int",
    [VTYPE_FLOAT] = "float",
};

// utils
int decl_var(const char *id, struct var var);
int decl_or_set_var(const char *id, struct var var);

struct var_entry *get_var(const char *id);
int set_var(const char *id, struct var var);

bool valid_id(const char *id);
bool compatible_types(var_type dest, var_type orig);

union var_val default_var_val(var_type type);

void initialize_interpreter(void) {
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        memset(variables[i].id, 0, VAR_ID_SIZE);
    }
}

struct var interpret_ast_node(const struct ast_node *node, FILE *fout);

struct var op_unary_arithmetic(const struct op_node *node, FILE *fout) {
    struct var a;
    a = interpret_ast_node(node->operands[0], fout);
    switch (node->op) {
    case '-':
        return var_negate(a);
    default:
        return NULL_VAR;
    }
}
struct var op_bin_arithmetic(const struct op_node *node, FILE *fout) {
    struct var a, b;
    a = interpret_ast_node(node->operands[0], fout);
    b = interpret_ast_node(node->operands[1], fout);

    switch (node->op) {
    case '-':
        return var_sub(a, b);
    case '+':
        return var_add(a, b);
    case '*':
        return var_mul(a, b);
    case '/':
        return var_div(a, b);
    default:
        return NULL_VAR;
    }
}

void op_assignment(const struct op_node *node, FILE *fout) {
    struct var var = interpret_ast_node(node->operands[1], fout);

    decl_or_set_var(node->operands[0]->id.id, var);
}

void op_decl(const struct op_node *node, FILE *fout) {
    struct var var;
    var.type = node->op;

    if (node->n_operands == 1)
        var.val = default_var_val(var.type);
    else
        var.val = interpret_ast_node(node->operands[1], fout).val;

    decl_var(node->operands[0]->id.id, var);
}

struct var interpret_ast_node(const struct ast_node *node, FILE *fout) {
    if (!node)
        return NULL_VAR;

    switch (node->type) {
    case NTYPE_ID:
        return get_var(node->id.id)->v;
    case NTYPE_CONST:
        return node->cst.v;
    case NTYPE_OP:
        switch (node->op.op) {
        case '-':
            if (node->op.n_operands == 1) {
                goto unary;
            }
        case '+':
        case '*':
        case '/':
            if (node->op.n_operands != 2) {
                sserror(
                    false,
                    "interpret_ast_node: binary arithmetic expects 2 operands");
                return NULL_VAR;
            }
            return op_bin_arithmetic(&node->op, fout);
        unary:
            return op_unary_arithmetic(&node->op, fout);

        case '=':
            if (node->op.n_operands != 2) {
                sserror(false,
                        "interpret_ast_node: assignment expects 2 operands");
                return NULL_VAR;
            }
            op_assignment(&node->op, fout);
            return NULL_VAR;

        /* print */
        case 'p':
            if (node->op.n_operands != 1) {
                sserror(false, "interpret_ast_node: print expects 1 operand");
                return NULL_VAR;
            }
            fprintf(fout, "print ");
            print_var(interpret_ast_node(node->op.operands[0], fout), fout);
            fprintf(fout, "\n");
            return NULL_VAR;

        /* declarations */
        case VTYPE_INT:
        case VTYPE_FLOAT:
            if (node->op.n_operands > 2) {
                sserror(false,
                        "interpret_ast_node: decl expects at most 2 operands");
                return NULL_VAR;
            }
            op_decl(&node->op, fout);
            return NULL_VAR;
        default:
            sserror(true, "interpret_ast_node: invalid operand\n");
            return NULL_VAR;
        }
    default:
        sserror(true, "interpret_ast_node: invalid node type\n");
        return NULL_VAR;
    }
}

void interpret_ast(const struct ast_node *ast, FILE *fout) {
    if (!ast) {
        sserror(true, "interpret_ast: null ast");
        return;
    }

    struct var result = interpret_ast_node(ast, fout);
    if (result.type == VTYPE_NULL ||
        (ast->type == NTYPE_OP && ast->op.op == 'p'))
        return;

    fprintf(fout, "= ");
    print_var(result, fout);
    fprintf(fout, "\n");
}

int decl_var(const char *id, struct var var) {
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
    free_entry->v = var;
    return 0;
}

int decl_or_set_var(const char *id, struct var var) {
    if (!valid_id(id))
        return 1;

    struct var_entry *free_entry = NULL;
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        if (strcmp(variables[i].id, id) == 0)
            return set_var(id, var);

        if (!free_entry && *variables[i].id == '\0')
            free_entry = &variables[i];
    }

    if (!free_entry) {
        sserror(false, "decl_or_set_var: max variables stored reached\n");
        return 1;
    }

    snprintf(free_entry->id, VAR_ID_SIZE, "%s", id);
    free_entry->v = var;
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

int set_var(const char *id, struct var var) {
    struct var_entry *entry;

    if (!(entry = get_var(id)))
        return 1;

    if (compatible_types(entry->v.type, var.type)) {
        entry->v.val = var.val;
        return 0;
    }

    sserror(false, "set_var: assigning %s to variable of type %s\n",
            vtype_names[var.type], vtype_names[entry->v.type]);
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
        return (union var_val){.ival = 0};

    case VTYPE_FLOAT:
        return (union var_val){.fval = 0.0f};

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
