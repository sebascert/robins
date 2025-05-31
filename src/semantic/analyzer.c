#include "semantic/analyzer.h"

#include "backend/arithmetic.h"
#include "semantic/argument.h"
#include "semantic/error.h"

bool arg_semantic_validate(argument_t type, struct literal* evaluated_arg)
{
    if (cast_lit(evaluated_arg, arg_types[type]) != 0) {
        sserror("invalid type '%s' for %s",
                literal_t_names[evaluated_arg->type], argument_t_names[type]);
        return false;
    }

    return user_arg_sv(type, evaluated_arg);
}
