include(`defs/noun.m4')dnl
include(`defs/politeness_level.m4')dnl
include(`defs/instruction.m4')dnl
include(`defs/argument.m4')dnl
dnl
ROBINS_NOUN(`robot')dnl
ROBINS_NOUN(`Robot')dnl
dnl
ROBINS_POLITENESS_LEVEL(1)dnl
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
ROBINS_INS(`mov', `move', `blocks')dnl
ROBINS_INS(`rot', `rotate', `degrees')dnl
dnl
include(`defs/assert.m4')dnl
