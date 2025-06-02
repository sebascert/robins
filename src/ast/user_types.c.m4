include(`config.m4')dnl
ifdef(`ROBINS_INS_MNEMONICS', , `errprint(`m4: ERROR: ROBINS_INS_MNEMONICS is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_ARG_TYPE_NAMES', , `errprint(`m4: ERROR: ROBINS_ARG_TYPE_NAMES is not defined.')
m4exit(1)')dnl
#include "ast/user_types.h"

const char *const instruction_t_mnemonics[] = {
    ROBINS_INS_MNEMONICS()
};

const char *const argument_t_names[] = {
    ROBINS_ARG_TYPE_NAMES()
};
