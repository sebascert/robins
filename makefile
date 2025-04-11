target := simplecalc

#cflags
clibs       := -lfl -ly
cflags      := -Wall -Werror
lexyacc_cflags :=

#dirs
src_dir     := src
build_dir   := build
tests_dir   := tests

#sources
lexer       := $(src_dir)/lexer.l
parser      := $(src_dir)/parser.y
sources     := $(filter-out $(lex_c) $(yacc_c), $(shell find $(src_dir) -name '*.c'))

#lexyacc
lex_c       := $(src_dir)/lex.yy.c
yacc_c      := $(src_dir)/y.tab.c
yacc_h      := $(src_dir)/y.tab.h
lexyacc_sources := $(lex_c) $(yacc_c) $(yacc_h)

# objects
objs        := $(sources:.c=.o)
lexyacc_objs   := $(lex_c:.c=.o) $(yacc_c:.c=.o)

#lexyacc flags
lexflags    ?=
yaccflags   ?=

all: $(target)

run: $(target)
	@echo "./$(build_dir)/$(target)"
	@./$(build_dir)/$(target) || exit "$$?"

test: $(target)
	@./test.sh "$(build_dir)/$(target)" "$(tests_dir)"

clean:
	@rm -f $(lexyacc_sources)
	@rm -f $(build_dir)/$(target)
	@find . -name '*.o' -delete

clean-all:
	@rm -rf $(build_dir)

#temp fix for compilation errors
$(target):
	@$(MAKE) --no-print-dir clean
	@$(MAKE) --no-print-dir $(build_dir)/$(target)

.PHONY: all clean run test $(target)

$(build_dir)/$(target): $(lexyacc_objs) $(objs) | $(build_dir)
	gcc $(clibs) $(objs) $(lexyacc_objs) -o $@

$(lex_c): $(lexer) $(yacc_h)
	lex $(lexflags) --outfile="$(lex_c)" -- $(lexer)

$(yacc_c) $(yacc_h): $(parser)
	bison $(yaccflags) --output="$(yacc_c)" --header="$(yacc_h)" -- $(parser)

%.o: %.c
	gcc $(cflags) -c $< -o $@

$(lexyacc_objs): $(lex_c) $(yacc_c)
$(lexyacc_objs): %.o: %.c
	gcc $(lexyacc_cflags) -c $< -o $@

$(build_dir):
	@mkdir -p $(build_dir)
