target := simplecalc

libs = -ll -ly
cflags = -Wall -Werror

src_dir := src
build_dir := build
tests_dir := tests

#compiled
lex_c := $(src_dir)/lex.yy.c
yacc_c := $(src_dir)/y.tab.c
yacc_h := $(src_dir)/y.tab.h

#sources
lexer   := $(src_dir)/lexer.l
parser  := $(src_dir)/parser.y
sources = $(shell find $(src_dir) -name '*.c')
sources := $(filter-out $(lex_c) $(yacc_c), $(sources))
objs = $(sources:.c=.o)

all: $(target)

run: $(target)
	@echo "./$(build_dir)/$(target)"
	@./$(build_dir)/$(target) || exit "$$?"

test: $(target)
	@./test.sh "$(build_dir)/$(target)" "$(tests_dir)"

clean:
	@rm -rf $(build_dir)
	@rm -rf $(test_out)
	@rm -f $(lex_c) $(yacc_c) $(yacc_h)
	@find . -name '*.o' -delete

$(target): $(build_dir)/$(target)

.PHONY: all clean run test $(target)

$(build_dir)/$(target): $(objs) $(lex_c) $(yacc_c) | $(build_dir)
	gcc $(lex_c) $(objs) $(yacc_c) $(libs) -o $@

$(lex_c): $(lexer) $(yacc_h)
	lex --outfile="$(lex_c)" $(lexer)

$(yacc_c) $(yacc_h): $(parser)
	yacc -d --output="$(yacc_c)" --header="$(yacc_h)" $(parser)

%.o: %.c
	gcc $(cflags) -c $< -o $@

$(build_dir):
	@mkdir -p $(build_dir)
