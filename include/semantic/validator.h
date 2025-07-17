#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast/nodes/literal.h"
#include "ast/user_types.h"

#include <stdbool.h>

// argument semantic validation
bool arg_semantic_validate(argument_t type, struct literal* evaluated_arg);

#endif /* ifndef SEMANTIC_ANALYZER_H */
