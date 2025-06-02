include(`config.m4')dnl
ifdef(`ROBINS_INS_TYPES', , `errprint(`m4: ERROR: ROBINS_INS_TYPES is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_ARG_TYPES', , `errprint(`m4: ERROR: ROBINS_ARG_TYPES is not defined.')
m4exit(1)')dnl
#ifndef AST_USER_TYPES_H
#define AST_USER_TYPES_H

extern const char *const instruction_t_mnemonics[];

typedef enum {
    ROBINS_INS_TYPES()
} instruction_t;

extern const char *const argument_t_names[];

typedef enum {
    ROBINS_ARG_TYPES()
} argument_t;

#endif /* ifndef AST_USER_TYPES_H */
