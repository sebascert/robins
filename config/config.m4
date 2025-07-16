dnl ROBINS CONFIGURATION
dnl
dnl Read the wiki configuration page for documentation on the options.
dnl If you do not follow the instructions your program may not compile, or the
dnl language recognized by it may be ambiguous.
dnl
dnl Add "dnl" at the end of every empty line, as this improves the formatting
dnl of m4 generated files.
dnl
dnl DO NOT REMOVE
dnl Includes for macro definitions.
include(`defs/noun.m4')dnl
include(`defs/conjunction.m4')dnl
include(`defs/politeness_level.m4')dnl
include(`defs/instruction.m4')dnl
include(`defs/argument.m4')dnl
dnl
dnl Nouns
ROBINS_NOUN(`robot')dnl
ROBINS_NOUN(`Robot')dnl
dnl
dnl Instruction conjunctions
ROBINS_CONJUNCTION(``,' then')dnl
ROBINS_CONJUNCTION(``,' and then')dnl
dnl
dnl Politeness Level
ROBINS_POLITENESS_LEVEL(1)dnl
dnl
dnl Arguments
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
dnl Instructions
ROBINS_INS(`mov', `move', `blocks')dnl
ROBINS_INS(`rot', `rotate', `degrees')dnl
dnl
dnl DO NOT REMOVE
dnl Asserts that the given configuration is valid.
include(`defs/assert.m4')dnl
