include(`config.m4')dnl
ifdef(`ROBINS_ARG_TYPE_MAP', , `errprint(`m4: ERROR: ROBINS_ARG_TYPE_MAP is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_ARG_SV_DECL', , `errprint(`m4: ERROR: ROBINS_ARG_SV_DECL is not defined.')
m4exit(1)')dnl
ifdef(`ROBINS_ARG_SV_CASES', , `errprint(`m4: ERROR: ROBINS_ARG_SV_CASES is not defined.')
m4exit(1)')dnl
#include "semantic/argument.h"
#include "semantic/error.h"

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
