# Compiler Lab

## Project Structure

```
.
├── build
│   └── simplecalc
├── src
│   ├── main.c
│   ├── lexer.l
│   ├── parser.y
│   ├── parse.c
│   ├── parse.h
│   ├── var.c
│   ├── var.h
│   ├── interpret.c
│   ├── interpret.h
│   └── serialize_ast.c
├── tests
│   └── ...
├── scripts
│   └── ...
└── makefile

```

## dependencies

The library `libbison-dev` is required for linking `liby.a`.

```bash
sudo apt update
sudo apt install flex bison libbison-dev
```

## Usage

compile to `build/simplecalc`:
```bash
make
```

run program:
```bash
make run
```

run tests:
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
