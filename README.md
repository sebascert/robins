# Robins (Robot Instruction Compiler)

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

The project depends on `flex` for lexical analysis and `bison` for syntactical
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
