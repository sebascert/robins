dnl Copyright (C) 2006, 2010-2012 Free Software Foundation, Inc.
dnl This file is free software; the Free Software Foundation
dnl gives unlimited permission to copy and/or distribute it,
dnl with or without modifications, as long as this notice is preserved.
dnl
divert(`-1')
# upcase(text)
# downcase(text)
# capitalize(text)
#   change case of text, simple version
define(`upcase', `translit(`$*', `a-z', `A-Z')')
define(`downcase', `translit(`$*', `A-Z', `a-z')')
define(`_capitalize',
       `regexp(`$1', `^\(\w\)\(\w*\)',
               `upcase(`\1')`'downcase(`\2')')')
define(`capitalize', `patsubst(`$1', `\w+', `_$0(`\&')')')
divert`'dnl
