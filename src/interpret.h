#ifndef INTERPRET_H
#define INTERPRET_H

#include "parser.h"

#include <stdbool.h>

#define VAR_ID_SIZE 10
#define MAX_VAR_ENTRIES 26

void initialize_interpreter(void);
void execute(struct ast_node *node);

void sserror(bool terminate, const char *s, ...);

#endif /* ifndef INTERPRET_H */
