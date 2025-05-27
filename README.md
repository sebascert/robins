# ROBINS (Robot Instruction Compiler)

ROBINS compiles natural language imperative instructions into assembly-like
commands. It is designed for lightweight applications that need to convert
natural language into a more manageable, structured encoding. Both the natural
language syntax and its mappings are fully configurable.

## Project Structure

```
.
├── build
│   └── robins
├── include
│   └── headers
├── src
│   └── TUs, lexer, parser
├── tests
│   └── ...
├── scripts
│   └── ...
└── makefile
```

## dependencies

- **argp**
- **flex**
- **bison**: libbison-dev

The project uses `flex` for lexical analysis and `bison` for syntactical
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
