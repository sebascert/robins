#ifndef AST_USER_TYPES_H
#define AST_USER_TYPES_H

extern const char *const instruction_t_mnemonics[];

typedef enum {
    INS_ROTATE, /* macroized */
    INS_MOVE,   /* macroized */
} instruction_t;

extern const char *const argument_t_names[];

typedef enum {
    ARG_DEGREES, /* macroized */
    ARG_BLOCKS,  /* macroized */
} argument_t;

#endif /* ifndef AST_USER_TYPES_H */
