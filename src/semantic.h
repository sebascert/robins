#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdarg.h>
#include <stdbool.h>

extern const char *const vtype_names[];

enum vtype {
    VTYPE_INT,
    VTYPE_FLOAT,
};

struct vval {
    enum vtype type;
    union {
        int ival;
        float fval;
    };
};

#define VAR_ID_SIZE 10
struct var_entry {
    struct vval val;
    char id[VAR_ID_SIZE];
};

#define MAX_VAR_ENTRIES 26
extern struct var_entry variables[MAX_VAR_ENTRIES];

void initialize_variables(void);

// return 1: null id
// return 1: id length exceeds VAR_ID_SIZE
// return 1: redeclaring var
// return 1: no free var entry
int decl_var(const char *id, struct vval val);

// return NULL: null id
// return NULL: id length exceeds VAR_ID_SIZE
// return NULL: non declared variable
struct var_entry *get_var(const char *id);

// return 1: different types
// return 1: non declared variable
int set_var(const char *id, struct vval val);

int decl_or_set_var(const char *id, struct vval val);

// semantic error
void sserror(bool terminate, const char *s, ...);

bool attempt_assingment(struct vval *dest, struct vval orig);

#endif /* ifndef SEMANTIC_H */
