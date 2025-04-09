# Compiler Lab

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

clean compiled and temp files:
```bash
make clean
```
