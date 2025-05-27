target  := robins

args    ?=
test    ?=

# dirs
src_dir     := src
include_dir := include
build_dir   := build
tests_dir   := tests
scripts_dir := scripts

# sources and headers
lexer               := $(src_dir)/frontend/lexer.l
parser              := $(src_dir)/frontend/parser.y
lex_i               := $(lexer:.l=.i)
yacc_i              := $(parser:.y=.i)
lex_c               := $(lexer:.l=.c)
yacc_c              := $(parser:.y=.c)
yacc_h              := $(parser:.y=.h)
lexyacc_sources     := $(lex_c) $(yacc_c)
lexyacc_genfiles    := $(lex_i) $(yacc_i) $(lexyacc_sources) $(yacc_h)

sources     := $(filter-out $(lexyacc_sources), $(shell find $(src_dir) -name '*.c'))
headers     := $(shell find $(include_dir) -name '*.h')

# objects
objs            := $(sources:.c=.o)
lexyacc_objs    := $(lexyacc_sources:.c=.o)

# compiler setup
CPP         := cpp
cppflags    := -P -I$(include_dir)
CC          := gcc
CSTD        := c99
CLINK       := -lfl -ly
CFLAGS      := -Wall -Wextra -std=$(CSTD) -I$(include_dir)
ifeq ($(BUILD),release)
	target += _release
	CFLAGS += -O2
else
	CFLAGS += -g
endif

#lexyacc flags
LEX             := flex
YACC            := bison
LEXYACC_CFLAGS  :=
LEXFLAGS        ?=
YACCFLAGS       ?=

CLANGDB   := compile_commands.json

MAKEFLAGS += --no-print-directory

# target rules
all: $(target)

$(target): $(build_dir)/$(target)

.PHONY: all $(target)

# utils rules
test: $(target)
	@./$(scripts_dir)/test.sh "$(realpath $(build_dir)/$(target))" "$(realpath $(tests_dir))"

add-test:
ifeq ($(strip $(test)),)
	@echo "missing test=<test>"
else
	@./$(scripts_dir)/add-test.sh "$(realpath $(tests_dir))" "$(test)"
endif

format:
	@clang-format -i $(headers) $(sources)

lint: $(CLANGDB)
	@clang-tidy $(headers) $(sources) -p .

clangdb: clean-clangdb
	@$(MAKE) $(CLANGDB)

.PHONY: test add-test format lint clangdb

# compilation rules
$(build_dir)/$(target): $(objs) $(lexyacc_objs) | $(build_dir)
	@$(CC) $(CFLAGS) $(CLINK) $^ -o $@

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(lex_i): $(lexer) $(yacc_h)
	@$(CPP) $(cppflags) $(lexer) -o $(lex_i)
$(yacc_i): $(parser)
	@$(CPP) $(cppflags) $(parser) -o $(yacc_i)

$(lex_c): $(lex_i)
	@$(LEX) $(LEXFLAGS) --outfile="$(lex_c)" -- $(lex_i)
$(yacc_c) $(yacc_h): $(yacc_i)
	@$(YACC) $(YACCFLAGS) --output="$(yacc_c)" --header="$(yacc_h)" -- $(yacc_i)

# $(lexyacc_objs): $(lex_c) $(yacc_c)
$(lexyacc_objs): %.o: %.c
	$(CC) $(LEXYACC_CFLAGS) -c $< -o $@

$(CLANGDB):
	@$(MAKE) clean
	@bear -- $(MAKE) $(objs)

$(build_dir):
	@mkdir -p $(build_dir)

# clean rules
clean:
	@rm -rf $(build_dir)
	@rm -f $(lexyacc_genfiles)
	@find . -name '*.o' -exec rm -f {} +

clean-clangdb:
	@rm -f $(CLANGDB)
	@rm -rf .cache/clangd

clean-all: clean clean-clangdb

.PHONY: clean clean-clangdb clean-all
