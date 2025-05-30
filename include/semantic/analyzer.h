#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "ast/nodes/argument.h"
#include "ast/nodes/literal.h"

#include <stdbool.h>

// semantic error
void sserror(const char* s, ...);

/* argument semantic validators */
bool arg_semantic_validate(argument_t type, struct literal* evaluated_arg);
extern const literal_t arg_types[];

bool arg_sv_degrees(int value); /* macroized */

#endif /* ifndef SEMANTIC_ANALYZER_H */
