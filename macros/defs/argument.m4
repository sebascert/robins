include(`utils/print.m4')dnl
include(`utils/stack.m4')dnl
include(`utils/capitalize.m4')dnl
dnl
define(`ROBINS_ARG_GRAMMAR_MACROS_FORMATTING',
`%token ARG_`'upcase($1)_UNIT
%type <nptr> arg_$1
')dnl
define(`ROBINS_ARG_GRAMMAR_RULES_FORMATTING',
`arg_$1:
    num_expr ARG_`'upcase($1)_UNIT { $$ = push_arg(ARG_`'upcase($1)`,' `$'1); }
;
')dnl
define(`ROBINS_ARG_UNIT_LEXEMAS_FORMATTING',
`"$3" { return ARG_`'upcase($1)_UNIT; }
')dnl
define(`ROBINS_ARG_TYPES_FORMATTING',
`ARG_`'upcase($1)`,'
')dnl
define(`ROBINS_ARG_TYPE_NAMES_FORMATTING',
`[ARG_`'upcase($1)] = "$1"`,'
')dnl
define(`ROBINS_ARG_TYPE_MAP_FORMATTING',
`[ARG_`'upcase($1)] = LITERAL_T_`'upcase($2)`,'
')dnl
define(`ROBINS_ARG_SV_DECL_FORMATTING',
`bool arg_sv_$1(ifelse($2,int,int,double) val)
{$4}
')dnl
define(`ROBINS_ARG_SV_CASES_FORMATTING',
`case ARG_`'upcase($1):
    return arg_sv_$1(evaluated_arg->val.ifelse($2,int,ival,rval));
')dnl
dnl
define(`ROBINS_ARG_GRAMMAR_MACROS',
`stack_foreach(`ROBINS_ARG_GRAMMAR_MACROS_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_ARG_GRAMMAR_RULES',
`stack_foreach(`ROBINS_ARG_GRAMMAR_RULES_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_ARG_UNIT_LEXEMAS',
`stack_foreach(`ROBINS_ARG_UNIT_LEXEMAS_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_ARG_TYPES',
`stack_foreach(`ROBINS_ARG_TYPES_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_ARG_TYPE_NAMES',
`stack_foreach(`ROBINS_ARG_TYPE_NAMES_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_ARG_TYPE_MAP',
`stack_foreach(`ROBINS_ARG_TYPE_MAP_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_ARG_SV_DECL',
`stack_foreach(`ROBINS_ARG_SV_DECL_STACK', `ROBINS_PRINT')')dnl
define(`ROBINS_ARG_SV_CASES',
`stack_foreach(`ROBINS_ARG_SV_CASES_STACK', `ROBINS_PRINT')')dnl
dnl
define(`ROBINS_ARG',
`ifelse($2, `int', `',
`ifelse($2, `real', `',
`errprint(`Error: invalid type "$2".
')m4exit(1)')')dnl
pushdef(`ROBINS_ARG_GRAMMAR_MACROS_STACK', ROBINS_ARG_GRAMMAR_MACROS_FORMATTING(`$1'))dnl
pushdef(`ROBINS_ARG_GRAMMAR_RULES_STACK', ROBINS_ARG_GRAMMAR_RULES_FORMATTING(`$1'))dnl
pushdef(`ROBINS_ARG_UNIT_LEXEMAS_STACK', ROBINS_ARG_UNIT_LEXEMAS_FORMATTING(`$1', `$2', `$3'))dnl
pushdef(`ROBINS_ARG_TYPES_STACK', ROBINS_ARG_TYPES_FORMATTING(`$1'))dnl
pushdef(`ROBINS_ARG_TYPE_NAMES_STACK', ROBINS_ARG_TYPE_NAMES_FORMATTING(`$1'))dnl
pushdef(`ROBINS_ARG_TYPE_MAP_STACK', ROBINS_ARG_TYPE_MAP_FORMATTING(`$1', `$2'))dnl
pushdef(`ROBINS_ARG_SV_DECL_STACK', ROBINS_ARG_SV_DECL_FORMATTING(`$1', `$2', `$3', `$4'))dnl
pushdef(`ROBINS_ARG_SV_CASES_STACK', ROBINS_ARG_SV_CASES_FORMATTING(`$1', `$2'))dnl
')dnl
