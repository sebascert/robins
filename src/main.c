#include <argp.h>
#include <stdio.h>

#include "interpret.h"
#include "parse.h"

// argument parsing with argp
struct args {
    unsigned int stdin : 1;
    /* modes */
    unsigned int graph : 1;
    unsigned int interpret : 1;

    char *source_file;
    char *output_file;
};

const char *argp_program_version = "simplecalc";
static char doc[] = "simple calculator program";
static char args_doc[] = "<sourcefile>\n--stdin";

#define ARG_GROUP(name, group) {0, 0, NULL, 0, name, group}
static struct argp_option options[] = {
    ARG_GROUP("io:", 1),
    {"stdin", 'i', NULL, 0, "input from stdin instead of <sourcefile>", 1},
    {"output", 'o', "FILE", 0, "output to FILE instead of stdout", 1},
    ARG_GROUP("modes:", 2),
    {"interpret", 'I', NULL, 0, "interpret input line by line, set by default",
     2},
    {"graph", 'g', NULL, 0, "generate graphviz graph of the AST", 2},
    {0}};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct args *args = state->input;

    switch (key) {
    case 'i':
        args->stdin = 1;
        break;
    case 'o':
        args->output_file = arg;
        break;
    case 'I':
        args->interpret = 1;
        break;
    case 'g':
        args->graph = 1;
        break;
    case ARGP_KEY_ARG:
        if (state->arg_num >= 1) {
            argp_error(state,
                       "expecting only <sourcefile> positional argument");
        }
        args->source_file = arg;
        break;
    case ARGP_KEY_END:
        if (!args->stdin && !args->source_file) {
            argp_usage(state);
        }
        if (args->stdin && args->source_file) {
            argp_error(state, "conflicting '%s' and --stdin",
                       args->source_file);
        }
        if (args->graph && args->interpret) {
            argp_error(state, "conflicting --graph and --interpret");
        }
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

static struct argp argp = {options, parse_opt, args_doc, doc};

int yyparse(void);
extern FILE *yyin;

ast_evaluator eval_ast_statement;

static FILE *fout;
void ast_graph(const struct ast_node *ast) { serialize_ast(ast, fout); }
void interpret(const struct ast_node *ast) { interpret_ast(ast, fout); }

int main(int argc, char **argv) {
    struct args args = {
        .stdin = 0,
        .interpret = 0,
        .graph = 0,
        .source_file = NULL,
        .output_file = NULL,
    };
    argp_parse(&argp, argc, argv, 0, 0, &args);

    if (!args.stdin) {
        yyin = fopen(args.source_file, "r");
        if (!yyin) {
            fprintf(stderr, "unable to open '%s'", args.source_file);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    if (args.output_file) {
        fout = fopen(args.output_file, "w");
        if (!fout) {
            fprintf(stderr, "unable to open '%s'", args.output_file);
            return 1;
        }
    } else {
        fout = stdout;
    }

    eval_ast_statement = args.graph ? ast_graph : interpret;

    fprintf(stderr, "simple calc program\n");

    int yycode = yyparse();

    fclose(yyin);
    fclose(fout);

    return yycode;
}
