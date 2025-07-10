include(`utils/robins/print.m4')dnl
include(`utils/gnu/stack.m4')dnl
include(`utils/gnu/quote.m4')dnl
include(`utils/gnu/foreachq.m4')dnl
include(`utils/gnu/forloop.m4')dnl
include(`utils/gnu/capitalize.m4')dnl
dnl
define(`ROBINS_INS_VERB_LEXEMAS_FORMATTING',
`"$2" { return INS_`'upcase($1)_VERB; }
')dnl
define(`ROBINS_INS_GRAMMAR_MACROS_FORMATTING',
`%token INS_`'upcase($1)_VERB
%type <nptr> ins_$1
')dnl
define(`ROBINS_INS_PARTIAL_GRAMMAR_RULE_FORMATTING',
`ifdef(`ROBINS_INS_PARTIAL_GRAMMAR_RULE_STACK',
`|   ', `    ')ins_$1 { $$ = `$'1; }
')dnl
define(`ROBINS_INS_GRAMMAR_RULES_FORMATTING',
`ins_$1:
    INS_`'upcase($1)_VERB foreachq(`x', quote(shift(shift($@))), `arg_`'x ')dnl
 { $$ = push_ins(INS_`'upcase($1), eval($# - 2)dnl
forloop(`i', `2', eval($# - 1), `, `$'i')); }
;
')dnl
define(`ROBINS_INS_TYPES_FORMATTING',
`INS_`'upcase($1)`,'
')dnl
define(`ROBINS_INS_MNEMONICS_FORMATTING',
`[INS_`'upcase($1)] = "$1"`,'
')dnl
dnl
define(`ROBINS_INS_VERB_LEXEMAS',
`stack_foreach(`ROBINS_INS_VERB_LEXEMAS_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_INS_GRAMMAR_MACROS',
`stack_foreach(`ROBINS_INS_GRAMMAR_MACROS_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_INS_PARTIAL_GRAMMAR_RULE',
`stack_foreach(`ROBINS_INS_PARTIAL_GRAMMAR_RULE_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_INS_GRAMMAR_RULES',
`stack_foreach(`ROBINS_INS_GRAMMAR_RULES_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_INS_TYPES',
`stack_foreach(`ROBINS_INS_TYPES_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_INS_MNEMONICS',
`stack_foreach(`ROBINS_INS_MNEMONICS_STACK', `ROBINS_PRINT')')dnl
dnl
define(`ROBINS_INS',
`pushdef(`ROBINS_INS_VERB_LEXEMAS_STACK', ROBINS_INS_VERB_LEXEMAS_FORMATTING(`$1', `$2'))dnl
pushdef(`ROBINS_INS_GRAMMAR_MACROS_STACK', ROBINS_INS_GRAMMAR_MACROS_FORMATTING(`$1'))dnl
pushdef(`ROBINS_INS_PARTIAL_GRAMMAR_RULE_STACK', ROBINS_INS_PARTIAL_GRAMMAR_RULE_FORMATTING(`$1'))dnl
pushdef(`ROBINS_INS_GRAMMAR_RULES_STACK', ROBINS_INS_GRAMMAR_RULES_FORMATTING($@))dnl
pushdef(`ROBINS_INS_TYPES_STACK', ROBINS_INS_TYPES_FORMATTING(`$1'))dnl
pushdef(`ROBINS_INS_MNEMONICS_STACK', ROBINS_INS_MNEMONICS_FORMATTING(`$1'))dnl
')dnl
