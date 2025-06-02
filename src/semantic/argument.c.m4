include(`config.m4')dnl
#include "semantic/argument.h"

const literal_t arg_types[] = {
    ROBINS_ARG_TYPE_MAP()
};

/* argument semantic validators */
ROBINS_ARG_SV_DECL()

bool user_arg_sv(argument_t type, struct literal* evaluated_arg)
{
    switch (type) {
        ROBINS_ARG_SV_CASES()
    }

    return false;
}
