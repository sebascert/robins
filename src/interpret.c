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

// utils
int alloc_var(const char *id, struct var var);
struct var_entry *get_var(const char *id);
int set_var(const char *id, struct var var);
bool is_valid_id(const char *id);

void initialize_interpreter(void) {
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        memset(variables[i].id, 0, VAR_ID_SIZE);
    }
}

// interpret

// operations
struct var op_unary_arithmetic(const struct op_node *node, FILE *fout);
struct var op_binary_arithmetic(const struct op_node *node, FILE *fout);
void op_assignment(const struct op_node *node, FILE *fout);
void op_decl(const struct op_node *node, FILE *fout);

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
        /* arithmetic */
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
            return op_binary_arithmetic(&node->op, fout);
        unary:
            if (node->op.n_operands != 1) {
                sserror(
                    false,
                    "interpret_ast_node: unary arithmetic expects 1 operand");
                return NULL_VAR;
            }
            return op_unary_arithmetic(&node->op, fout);

        /* assignment */
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
                sserror(false, "interpret_ast_node: declaration expects at "
                               "most 2 operands\n");
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
        sserror(true, "interpret_ast: null ast\n");
        return;
    }

    struct var result = interpret_ast_node(ast, fout);

    // print the result of an expression if not null and not print op
    if (result.type == VTYPE_NULL ||
        (ast->type == NTYPE_OP && ast->op.op == 'p'))
        return;

    fprintf(fout, "= ");
    print_var(result, fout);
    fprintf(fout, "\n");
}

struct var op_unary_arithmetic(const struct op_node *node, FILE *fout) {
    struct var a;
    a = interpret_ast_node(node->operands[0], fout);
    switch (node->op) {
    case '-':
        return negate_var(a);
    default:
        return NULL_VAR;
    }
}

struct var op_binary_arithmetic(const struct op_node *node, FILE *fout) {
    struct var a, b, result;
    a = interpret_ast_node(node->operands[0], fout);
    b = interpret_ast_node(node->operands[1], fout);

    switch (node->op) {
    case '-':
        return sub_var(a, b);
    case '+':
        return add_var(a, b);
    case '*':
        return mul_var(a, b);
    case '/':
        result = div_var(a, b);
        if (result.type == VTYPE_NULL)
            sserror(false, "op_binary_arithmetic: division by zero\n");

        return result;
    default:
        return NULL_VAR;
    }
}

void op_assignment(const struct op_node *node, FILE *fout) {
    const char *id = node->operands[0]->id.id;
    struct var var = interpret_ast_node(node->operands[1], fout);

    if (set_var(id, var) == 0)
        return;

    alloc_var(id, var);
}

void op_decl(const struct op_node *node, FILE *fout) {
    const char *id = node->operands[0]->id.id;
    struct var init;

    if (get_var(id)) {
        sserror(false, "op_decl: redeclaring '%s'\n", id);
        return;
    }

    if (node->n_operands == 1) {
        init.type = node->op;
        init.val = type_default_val(init.type);
    } else {
        init = interpret_ast_node(node->operands[1], fout);
        if (cast_var(init, node->op) != 0) {
            sserror(false, "op_decl: incompatible %s to %s cast\n",
                    vtype_names[init.type], vtype_names[node->op]);
            return;
        }
    }

    alloc_var(id, init);
}

int alloc_var(const char *id, struct var var) {
    if (!is_valid_id(id))
        return 1;

    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        if (*variables[i].id != '\0')
            continue;

        snprintf(variables[i].id, VAR_ID_SIZE, "%s", id);
        variables[i].v = var;
        return 0;
    }

    sserror(false, "alloc_var: max variables stored reached\n");
    return 1;
}

struct var_entry *get_var(const char *id) {
    if (!is_valid_id(id))
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

    if (cast_var(entry->v, var.type) != 0) {
        sserror(false, "set_var: incompatible %s to %s cast\n",
                vtype_names[var.type], vtype_names[entry->v.type]);
        return 1;
    }

    entry->v.val = var.val;
    return 0;
}

bool is_valid_id(const char *id) {
    if (!id) {
        sserror(true, "is_valid_id: null id string\n");
        return false;
    }

    if (strlen(id) >= VAR_ID_SIZE) {
        sserror(false, "is_valid_id: id '%s' is too long\n", id);
        return false;
    }

    return true;
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
