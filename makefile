build_dir := build
target := simplecalc

lex_source := simplecalc.l
lex_product := lex.yy.c

yacc_source := simplecalc.y
yacc_product := y.tab.c
yacc_headers := y.tab.h

all: $(build_dir)/$(target)

run: $(build_dir)/$(target)
	@echo "./$(build_dir)/$(target)"
	@./$(build_dir)/$(target) $(ARGS) || echo "exit with status code $$?"

$(build_dir)/$(target): $(lex_product) $(yacc_product) | $(build_dir)
	gcc $(lex_product) $(yacc_product) -ly -ll -o $@

$(lex_product): $(yacc_product) $(lex_source)
	lex $(lex_source)

$(yacc_product): $(yacc_source)
	yacc -d $(yacc_source)

$(build_dir):
	@mkdir -p $(build_dir)

clean:
	@rm -rf $(build_dir)
	@rm -f $(lex_product) $(yacc_product) $(yacc_headers)

.PHONY: all clean
