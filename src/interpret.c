#include "interpret.h"

#include "parser.h"

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
int decl_var(const char *id, union var_val val);
int decl_or_set_var(const char *id, var_type type, union var_val val);
struct var_entry *get_var(const char *id);
int set_var(const char *id, var_type type, union var_val val);
bool valid_id(const char *id);
bool compatible_types(var_type dest, var_type orig);
union var_val default_var_val(var_type type);

void execute(const struct ast_node *node) {}

void initialize_interpreter(void) {
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        memset(variables[i].id, 0, VAR_ID_SIZE);
    }
}

int decl_var(const char *id, union var_val val) {
    if (!valid_id(id))
        return 1;

    struct var_entry *free_entry = NULL;
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        if (strcmp(variables[i].id, id) == 0) {
            sserror(false, "redeclaring '%s'\n", id);
            return 1;
        }

        if (!free_entry && *variables[i].id == '\0')
            free_entry = &variables[i];
    }

    if (!free_entry) {
        sserror(false, "max variables stored reached");
        return 1;
    }

    snprintf(free_entry->id, VAR_ID_SIZE, "%s", id);
    free_entry->val = val;

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
        sserror(false, "max variables stored reached");
        return 1;
    }

    snprintf(free_entry->id, VAR_ID_SIZE, "%s", id);
    free_entry->val = val;

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

    sserror(false, "assigning %s to variable of type %s\n", variables[type],
            variables[var->type]);
    return 1;
}

bool valid_id(const char *id) {
    if (!id) {
        sserror(true, "null id string\n");
        return false;
    }

    if (strlen(id) >= VAR_ID_SIZE) {
        sserror(false, "id '%s' is too long\n", id);
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
        sserror(true, "unknown type '%s' has no default value",
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
