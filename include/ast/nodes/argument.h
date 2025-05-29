#ifndef AST_NODES_ARGUMENT_H
#define AST_NODES_ARGUMENT_H

extern const char *const argument_t_names[];

typedef enum {
    ARG_DEGREES, /* macroized */
} argument_t;

struct astnode;
struct argument {
    argument_t type;
    struct astnode *resolution;
};

#endif /* ifndef AST_NODES_ARGUMENT_H */
