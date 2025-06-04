# ROBINS (Robot Instruction Compiler)

ROBINS compiles natural language imperative instructions into assembly-like
commands. It is designed for lightweight applications that need to convert
natural language into a more manageable, structured encoding. Both the natural
language syntax and its mappings are configurable.

## Dependencies

- **GNU M4 v1.4.x**
- **argp**
- **flex v2.x**
- **bison v3.x**

ROBINS uses `flex` for lexical analysis and `bison` for syntactical analysis,
(in ubuntuthe library `libbison-dev` is required for linking `liby.a`). GNU M4
is used to generate code for the user configured language.

Example installation on Ubuntu:

```bash
sudo apt update
sudo apt install flex bison libbison-dev m4
```

## Usage

Configure the language parsed by ROBINS in `macros/config.m4`, each
configuration option contains documentation on it's usage in the default config
file.

> [GNU M4 manual on M4 syntax](https://www.gnu.org/software/m4/manual/m4.html#Syntax)

Read the [wiki](pending) for the full capabilities and limitations of languages
parsed by ROBINS. The default config file contains a simple usage of ROBINS for
the parsing of language with this instructions:

```
// may have have robot capitalized or not
// instructions may have polite words preceding them or not
robot please move 4 mtr.
Robot rotate 180 deg.
robot please rotate 180 deg, then move 10 mtr.
Robot move 12 mtr, then rotate 0 deg.
```

After installing the [dependencies](#dependencies), compile ROBINS with:

```bash
make # compiled to build/robins
```

Run the `--help` flag for the available command line options:

```bash
./build/robins --help
```

clean compiled and temp files:

```bash
make clean
```

## Tests

Run entire test suite:

```bash
make test
```
