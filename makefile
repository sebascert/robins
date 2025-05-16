target := robins

args ?=
test ?=

#cflags
clibs           := -lfl -ly
cflags          := -Wall -Werror -g
flexyacc_cflags :=

#dirs
src_dir     := src
build_dir   := build
tests_dir   := test
scripts_dir := scripts

#flexyacc
flex_c      := $(src_dir)/lex.yy.c
yacc_c      := $(src_dir)/y.tab.c
yacc_h      := $(src_dir)/y.tab.h
flexyacc_sources := $(flex_c) $(yacc_c)

#sources
lexer       := $(src_dir)/lexer.l
parser      := $(src_dir)/parser.y
sources     := $(filter-out $(flexyacc_sources), $(shell find $(src_dir) -name '*.c'))

# objects
objs            := $(sources:.c=.o)
flexyacc_objs   := $(flexyacc_sources:.c=.o)

#flexyacc flags
flexflags   ?=
yaccflags   ?=

all: $(target)

run: $(target)
	@echo "./$(build_dir)/$(target)"
	@./$(build_dir)/$(target) $(args)

test: $(target)
	@./$(scripts_dir)/test.sh "$(realpath $(build_dir)/$(target))" "$(realpath $(tests_dir))"

add-test:
ifeq ($(strip $(test)),)
	@echo "missing test=<test>"
else
	@./$(scripts_dir)/add-test.sh "$(realpath $(tests_dir))" "$(test)"
endif

clean:
	@rm -f $(flexyacc_sources) $(yacc_h)
	@rm -f $(build_dir)/$(target)
	@find . -name '*.o' -delete

clean-all:
	@rm -rf $(build_dir)

$(target): $(build_dir)/$(target)

.PHONY: all run test add-test clean clean-all $(target)

$(build_dir)/$(target): $(objs) $(flexyacc_objs) | $(build_dir)
	gcc $(cflags) $(clibs) $(objs) $(flexyacc_objs) -o $@

$(flex_c): $(flexer) $(yacc_h)
	flex $(flexflags) --outfile="$(flex_c)" -- $(lexer)

$(yacc_c) $(yacc_h): $(parser)
	bison $(yaccflags) --output="$(yacc_c)" --header="$(yacc_h)" -- $(parser)

%.o: %.c
	gcc $(cflags) -c $< -o $@

$(flexyacc_objs): $(flex_c) $(yacc_c)
$(flexyacc_objs): %.o: %.c
	gcc $(flexyacc_cflags) -c $< -o $@

$(build_dir):
	@mkdir -p $(build_dir)
