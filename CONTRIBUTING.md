# Contributing

Thank you for considering contributing! If you've found a bug or have a
suggestion, please open an issue. If you'd like to contribute code, feel free to
open a pull request.

Make sure that the changes you want to upload pass the [tests](#tests), the
sources are [formatted](#formatting) correctly, and there are no new
[linting](#linting) issues.

Project file structure:

```bash
├── config
│  └── config.m4
├── include # Headers mirroring src
├── macros
│  ├── defs # Macro definitions used in config
│  └── utils # GNU M4 utilities
├── scripts
│  ├── add-test.sh
│  └── test.sh
├── src
│  ├── ast # Abstract Syntax Tree
│  ├── backend # Translation
│  ├── frontend # lexical and syntactical analysis
│  ├── semantic # semantic validation
│  └── main.c
├── tests # Tests on builded binary
└── wiki # Wiki submodule with markdown sources
```

## Developer Dependencies

- [bear](https://github.com/rizsotto/Bear)
- [valgrind](https://valgrind.org/)
- [clang-tidy](https://clang.llvm.org/extra/clang-tidy/)
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html)
- [graphviz](https://graphviz.org/) (only for rendering the AST)

## Tests

Use the scripts `scripts/add-test.sh` for adding scripts and `scripts/test.sh`
for executing tests, refer to their usage output for their usage instructions.

Execute the entire test suite:

```bash
make test
```

## Linting

Run linter, do it after building to lint M4 source/header outputs:

```bash
make lint
```

## Formatting

Formatting C sources:

```bash
make format
```

Formatting markdown sources:

```bash
find . -name "*.md" -exec prettier --write {} \;
```
