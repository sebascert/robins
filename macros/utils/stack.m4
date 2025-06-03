dnl Copyright (C) 2006, 2010-2012 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.
dnl
divert(`-1')
# stack_foreach(macro, action)
# Invoke ACTION with a single argument of each definition
# from the definition stack of MACRO, starting with the oldest.
define(`stack_foreach',
`_stack_reverse(`$1', `tmp-$1')'dnl
`_stack_reverse(`tmp-$1', `$1', `$2(defn(`$1'))')')
# stack_foreach_lifo(macro, action)
# Invoke ACTION with a single argument of each definition
# from the definition stack of MACRO, starting with the newest.
define(`stack_foreach_lifo',
`_stack_reverse(`$1', `tmp-$1', `$2(defn(`$1'))')'dnl
`_stack_reverse(`tmp-$1', `$1')')
define(`_stack_reverse',
`ifdef(`$1', `pushdef(`$2', defn(`$1'))$3`'popdef(`$1')$0($@)')')
divert`'dnl
