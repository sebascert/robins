include(`config.m4')dnl
#include "ast/user_types.h"

const char *const instruction_t_mnemonics[] = {
    ROBINS_INS_MNEMONICS()
};

const char *const argument_t_names[] = {
    ROBINS_ARG_TYPE_NAMES()
};
