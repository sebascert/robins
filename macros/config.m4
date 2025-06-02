dnl ROBINS CONFIGURATION
dnl
dnl Add "dnl" at the end of every statement or empty line, as this improves
dnl formatting in m4 generated files.
dnl
dnl parser macros
define(`ROBINS_INS_GRAMMAR_MACROS',
`%token INS_ROTATE_VERB
%type <nptr> ins_rotate
%token INS_MOVE_VERB
%type <nptr> ins_move')dnl
dnl
define(`ROBINS_ARG_GRAMMAR_MACROS',
`%token ARG_DEGREES_UNIT
%type <nptr> arg_degrees
%token ARG_BLOCKS_UNIT
%type <nptr> arg_blocks')dnl
dnl
define(`ROBINS_INS_GRAMMAR_RULE_STRUCTURE',
   `partial_ins                 { $$ = `$'1; }
|   POLITE_WORDS partial_ins    { $$ = `$'2; }')dnl
dnl
define(`ROBINS_INS_PARTIAL_GRAMMAR_RULE',
   `ins_rotate                  { $$ = `$'1; }
|   ins_move                    { $$ = `$'1; }')dnl
dnl
define(`ROBINS_INS_GRAMMAR_RULES',
`ins_rotate:
    INS_ROTATE_VERB arg_degrees { $$ = push_ins(INS_ROTATE, 1, `$'2); }
;
ins_move:
    INS_MOVE_VERB arg_blocks    { $$ = push_ins(INS_MOVE, 1, `$'2); }
;')dnl
dnl
define(`ROBINS_ARG_GRAMMAR_RULES',
`arg_degrees:
    num_expr ARG_DEGREES_UNIT   { $$ = push_arg(ARG_DEGREES, `$'1); }
;
arg_blocks:
    num_expr ARG_BLOCKS_UNIT   { $$ = push_arg(ARG_BLOCKS, `$'1); }
;')dnl
dnl
dnl lexer macros
dnl
define(`ROBINS_NOUN_LEXEMAS',
`"Robot"                         { return NOUN; }
"robot"                         { return NOUN; }')dnl
dnl
define(`ROBINS_POLITE_WORDS_LEXEMAS',
`"please"                        { return POLITE_WORDS; }
"would you please"              { return POLITE_WORDS; }')dnl
dnl
define(`ROBINS_INS_VERB_LEXEMAS',
`"rotate"                        { return INS_ROTATE_VERB; }
"move"                          { return INS_MOVE_VERB; }')dnl
dnl
define(`ROBINS_ARG_UNIT_LEXEMAS',
`"deg"                           { return ARG_DEGREES_UNIT; }
"blocks"                        { return ARG_BLOCKS_UNIT; }')dnl
dnl
dnl AST macros
dnl
define(`ROBINS_INS_TYPES',
   `INS_ROTATE,
    INS_MOVE,')dnl
dnl
define(`ROBINS_ARG_TYPES',
   `ARG_DEGREES,
    ARG_BLOCKS,')dnl
dnl
define(`ROBINS_INS_MNEMONICS',
   `[INS_ROTATE]= "rot",
    [INS_MOVE] = "mov",')dnl
dnl
define(`ROBINS_ARG_TYPE_NAMES',
   `[ARG_DEGREES] = "degrees",
    [ARG_BLOCKS] = "blocks",')dnl
dnl
dnl semantic macros
dnl
define(`ROBINS_ARG_TYPE_MAP',
   `[ARG_DEGREES] = LITERAL_T_INT,
    [ARG_BLOCKS] = LITERAL_T_INT,')dnl
dnl
define(`ROBINS_ARG_SV_DECL',
`bool arg_sv_degrees(int value)
{
    (void)value;
    return true;
}

bool arg_sv_blocks(int value)
{
    (void)value;
    return true;
}')dnl
dnl
define(`ROBINS_ARG_SV_CASES',
       `case ARG_DEGREES:
            return arg_sv_degrees(evaluated_arg->val.ival);
        case ARG_BLOCKS:
            return arg_sv_blocks(evaluated_arg->val.ival);')dnl
