define(`ROBINS_POLITE_WORDS_LEXEMAS_0',
`define(`ROBINS_POLITE_WORDS_LEXEMAS',`dnl')dnl')dnl
dnl
define(`ROBINS_POLITE_WORDS_LEXEMAS_12',
`define(`ROBINS_POLITE_WORDS_LEXEMAS',
`"please" { return POLITE_WORDS; }
"would you please" { return POLITE_WORDS; }')dnl')dnl
dnl
define(`ROBINS_INS_GRAMMAR_RULE_STRUCTURE_0',
`define(`ROBINS_INS_GRAMMAR_RULE_STRUCTURE',
   `partial_ins { $$ = `$'1; }')')dnl
dnl
define(`ROBINS_INS_GRAMMAR_RULE_STRUCTURE_1',
    `define(`ROBINS_INS_GRAMMAR_RULE_STRUCTURE',
`    partial_ins { $$ = `$'1; }
|   POLITE_WORDS partial_ins { $$ = `$'2; }')')dnl
dnl
define(`ROBINS_INS_GRAMMAR_RULE_STRUCTURE_2',
    `define(`ROBINS_INS_GRAMMAR_RULE_STRUCTURE',
`    POLITE_WORDS partial_ins { $$ = `$'2; }')')dnl
dnl
define(`ROBINS_POLITENESS_LEVEL',
`ifelse(`$1', `0',
`ROBINS_POLITE_WORDS_LEXEMAS_0()
ROBINS_INS_GRAMMAR_RULE_STRUCTURE_0()',
       `$1', `1',
`ROBINS_POLITE_WORDS_LEXEMAS_12()
ROBINS_INS_GRAMMAR_RULE_STRUCTURE_1()',
       `$1', `2',
`ROBINS_POLITE_WORDS_LEXEMAS_12()
ROBINS_INS_GRAMMAR_RULE_STRUCTURE_2()',
`errprint(`m4: ERROR: Invalid politeness level: $1')
m4exit(1)')')dnl
