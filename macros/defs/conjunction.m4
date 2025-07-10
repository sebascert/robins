include(`utils/gnu/stack.m4')dnl
dnl
define(`ROBINS_CONJUNCTION_LEXEMA_FORMATTING',
`"$1" { return INS_CONJUNCTION; }
')dnl
dnl
define(`ROBINS_CONJUNCTION_LEXEMAS',
`stack_foreach(`ROBINS_CONJUNCTION_STACK', `ROBINS_CONJUNCTION_LEXEMA_FORMATTING')')dnl
dnl
define(`ROBINS_CONJUNCTION',
`ifelse(`$1', `',
`errprint(`ROBINS_CONJUNCTION: empty conjuntion')
m4exit(1)',
`pushdef(`ROBINS_CONJUNCTION_STACK', `$1')')')dnl
