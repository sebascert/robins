#include "semantic.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *const vtype_names[] = {
    [VTYPE_INT] = "int",
    [VTYPE_FLOAT] = "float",
};

struct var_entry variables[MAX_VAR_ENTRIES];

void initialize_variables(void) {
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        memset(variables[i].id, 0, VAR_ID_SIZE);
    }
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

int decl_var(const char *id, struct vval val) {
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

struct var_entry *get_var(const char *id) {
    if (!valid_id(id))
        return NULL;

    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        if (strcmp(variables[i].id, id) == 0)
            return &variables[i];
    }

    sserror(false, "var not declared '%s'\n", id);
    return NULL;
}

int set_var(const char *id, struct vval val) {
    struct var_entry *var;

    if (!(var = get_var(id)))
        return 1;

    if (attempt_assingment(&var->val, val))
        return 0;

    sserror(false, "assigning %s to variable of type %s\n",
            vtype_names[val.type], vtype_names[var->val.type]);
    return 1;
}

int decl_or_set_var(const char *id, struct vval val) {
    if (!valid_id(id))
        return 1;

    struct var_entry *free_entry = NULL;
    for (size_t i = 0; i < MAX_VAR_ENTRIES; i++) {
        if (strcmp(variables[i].id, id) == 0)
            return set_var(id, val);

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

bool attempt_assingment(struct vval *dest, struct vval orig) {
    if (dest->type == orig.type) {
        *dest = orig;
    }

    switch (dest->type) {
    case VTYPE_INT:
        return false;

    case VTYPE_FLOAT:
        if (orig.type == VTYPE_INT) {
            dest->fval = (float)orig.ival;
            return true;
        }
        return false;

    default:
        return false;
    }
}
