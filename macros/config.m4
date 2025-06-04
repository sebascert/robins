dnl ROBINS CONFIGURATION
dnl
dnl Add "dnl" at the end of every empty line, as this improves the formatting
dnl of m4 generated files.
dnl
dnl READ THE INSTRUCTIONS CAREFULLY.
dnl If you do not follow the instructions your program may not compile, or the
dnl language recognized by it may be ambiguous.
dnl
dnl DO NOT REMOVE.
dnl Includes for macro definitions.
include(`defs/noun.m4')dnl
include(`defs/politeness_level.m4')dnl
include(`defs/instruction.m4')dnl
include(`defs/argument.m4')dnl
dnl
dnl Nouns for the hypothetical robot. Words registered as nouns become reserved
dnl words. Every valid statement must start with some noun.
dnl
dnl ROBINS_NOUN(`noun')
dnl nouns: string of noun.
dnl
ROBINS_NOUN(`robot')dnl
ROBINS_NOUN(`Robot')dnl
dnl
dnl Politeness level of the language recognized.
dnl
dnl ROBINS_POLITENESS_LEVEL(level)
dnl level: one of the following integers:
dnl     (0) no polite words accepted.
dnl     (1) both polite and not polite words accepted.
dnl     (2) onlu polite words accepted.
dnl
ROBINS_POLITENESS_LEVEL(1)dnl
dnl
dnl Argument Definitions.
dnl
dnl ROBINS_ARG(`arg_id', `type', `unit', `semantic_validation')
dnl arg_id: argument identifier for referencing in other macros,
dnl         must be a valid C variable name (otherwise undefined behaviour).
dnl type: argument type, see valid types below.
dnl unit: suffix words after the argument value.
dnl semantic_validation: C function predicate receiving val of corresponding
dnl                      argument type, returning false means the argument is
dnl                      semantically rejected (without {}).
dnl
dnl valid types:
dnl - int: integer number, int C type.
dnl - real: real number, double C type.
dnl
ROBINS_ARG(`degrees', `int', `deg', `
    if (val % 90 == 0)
        return true;
    sserror("invalid degrees: %", val);
    return false;
')dnl
ROBINS_ARG(`blocks', `int', `mtr', `
    (void) val;
    return true;
')dnl
dnl
dnl definitions of instructions.
dnl
dnl ROBINS_INS(`mnemonic', `verb', `argument'...)
dnl mnemonic: mnemonic for compiled instruction and identifier,
dnl           must be a valid C variable name (otherwise undefined behaviour).
dnl verb: word token for this instructions.
dnl argument: argument of instruction.
dnl
ROBINS_INS(`mov', `move', `blocks')dnl
ROBINS_INS(`rot', `rotate', `degrees')dnl
