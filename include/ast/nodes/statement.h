#ifndef AST_NODES_STATEMENT_H
#define AST_NODES_STATEMENT_H

#include <stddef.h>

#define STMT_INIT_INS_SIZE 1

struct astnode;
struct statement {
    size_t n_ins;
    size_t ins_vec_size;
    struct astnode** instructions;
};

#endif /* ifndef AST_NODES_STATEMENT_H */
