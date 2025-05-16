# Robins (Robot Instruction Compiler)

## Project Structure

```
.
├── makefile
├── build
│   └── robins
├── src
│   ├── main.c
│   ├── lexer.l
│   ├── parser.y
│   ├── parse.c
│   ├── parse.h
│   ├── code_generator.c
│   ├── code_generator.h
│   └── serialize_ast.c
├── tests
│   └── ...
├── scripts
│   └── ...
└── doc
    └── ...
```

## dependencies

The project depends on `flex` for lexical analysis and `bison` syntactical
analysis, the library `libbison-dev` is required for linking `liby.a`.

Example installation on Ubuntu:

```bash
sudo apt update
sudo apt install flex bison libbison-dev
```

## Compilation and Usage

compile to `build/robins`:

```bash
make
```

run program:

```bash
make run
```

run test suite:

```bash
make test
```

add test:

```bash
make add-test test=<test>
```

clean compiled and temp files:

```bash
make clean
```
