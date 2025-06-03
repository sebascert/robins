include(`utils/stack.m4')dnl
dnl
define(`ROBINS_NOUN_LEXEMA_FORMATTING',
`"$1" { return NOUN; }
')dnl
dnl
define(`ROBINS_NOUN_LEXEMAS',
`stack_foreach(`ROBINS_NOUN_STACK', `ROBINS_NOUN_LEXEMA_FORMATTING')')dnl
dnl
define(`ROBINS_NOUN',
`ifelse(`$1', `',
`errprint(`ROBINS_NOUN: empty noun')
m4exit(1)',
`pushdef(`ROBINS_NOUN_STACK', `$1')')')dnl
