target  := robins

args    ?=
test    ?=

# dirs
src_dir        := src
include_dir    := include
config_dir     ?= config
m4_include_dir := macros
build_dir      := build
tests_dir      := tests
scripts_dir    := scripts

# sources and headers
lexer               := $(src_dir)/frontend/lexer.l
parser              := $(src_dir)/frontend/parser.y
lex_c               := $(lexer:.l=.c)
yacc_c              := $(parser:.y=.c)
yacc_h              := $(parser:.y=.h)
lexyacc_sources     := $(lex_c) $(yacc_c)
lexyacc_genfiles    := $(lexyacc_sources) $(yacc_h)

m4_macros   := $(shell find $(m4_include_dir) -name '*.m4')
m4_sources  := $(patsubst %.c.m4, %.c, $(shell find $(src_dir) -name '*.c.m4'))
m4_headers  := $(patsubst %.h.m4, %.h, $(shell find $(include_dir) -name '*.h.m4'))
m4_genfiles := $(patsubst %.m4, %, $(shell find $(src_dir) $(include_dir) -name '*.m4'))

sources     := $(filter-out $(lexyacc_sources), $(shell find $(src_dir) -name '*.c'))
sources     += $(m4_sources)
headers     := $(shell find $(include_dir) -name '*.h')
headers     += $(m4_headers)

# objects
objs            := $(sources:.c=.o)
lexyacc_objs    := $(lexyacc_sources:.c=.o)

# compiler setup
CC          := gcc
CSTD        := c99
CLINK       := -lfl -ly
CFLAGS      := -Wall -Wextra -std=$(CSTD) -I$(include_dir)
ifeq ($(BUILD),release)
	target := robins_release
	CFLAGS += -O2
else
	CFLAGS += -g
endif

# m4 setup
M4          := m4
M4FLAGS     := -I$(m4_include_dir) -I$(config_dir)

# lex and yacc flags
LEX             := flex
YACC            := bison
LEXYACC_CFLAGS  := -I$(include_dir)
LEXFLAGS        :=
YACCFLAGS       :=

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

format: $(headers) $(sources)
	@clang-format -i $(headers) $(sources)

lint: $(CLANGDB) $(headers) $(sources)
	@clang-tidy -p . $(headers) $(sources)

clangdb: clean-clangdb
	@$(MAKE) $(CLANGDB)

.PHONY: test add-test format lint clangdb

# compilation rules
$(build_dir)/$(target): $(objs) $(lexyacc_objs) | $(build_dir)
	@$(CC) $(CFLAGS) $(CLINK) $^ -o $@
	@echo "compiled to: $@"

%.o: %.c | $(headers)
	@$(CC) $(CFLAGS) -c $< -o $@

$(lex_c): $(lexer) $(yacc_h)
	@$(LEX) $(LEXFLAGS) --outfile="$(lex_c)" -- $(lexer)
	@echo "compiled lexical analyzer"
$(yacc_c) $(yacc_h): $(parser)
	@$(YACC) $(YACCFLAGS) --output="$(yacc_c)" --header="$(yacc_h)" -- $(parser)
	@echo "compiled syntactical analyzer"

$(lexyacc_objs): %.o: %.c
	@$(CC) $(LEXYACC_CFLAGS) -c $< -o $@

$(CLANGDB):
	@$(MAKE) clean
	@bear -- $(MAKE) $(objs)

$(build_dir):
	@mkdir -p $(build_dir)

# m4 macro processing

%.c: %.c.m4 $(m4_macros)
	@$(M4) $(M4FLAGS) $< > $@

%.h: %.h.m4 $(m4_macros)
	@$(M4) $(M4FLAGS) $< > $@

%.l: %.l.m4 $(m4_macros)
	@$(M4) $(M4FLAGS) $< > $@

%.y: %.y.m4 $(m4_macros)
	@$(M4) $(M4FLAGS) $< > $@

# clean rules
clean:
	@rm -rf $(build_dir)
	@rm -f $(m4_genfiles)
	@rm -f $(lexyacc_genfiles)
	@find . -name '*.o' -exec rm -f {} +

clean-clangdb:
	@rm -f $(CLANGDB)
	@rm -rf .cache/clangd

clean-all: clean clean-clangdb

.PHONY: clean clean-clangdb clean-all
