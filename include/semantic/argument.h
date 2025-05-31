#ifndef SEMANTIC_ARGUMENT_H
#define SEMANTIC_ARGUMENT_H

#include "ast/nodes/literal.h"
#include "ast/user_types.h"

#include <stdbool.h>

extern const literal_t arg_types[];

/* user specified argument semantic validators */
bool user_arg_sv(argument_t type, struct literal* evaluated_arg);

#endif /* ifndef SEMANTIC_ARGUMENT_H */
