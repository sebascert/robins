# ROBINS (Robot Instruction Compiler)

ROBINS translates natural language imperative commands into assembly-like
instructions. It is intended for lightweight conversion of this subset of
natural language into a more manageable, structured encoding. ROBINS is
configurable, allowing customization of both its syntax and translation output.

<span style="color: #D27FA6;">Robot</span> <span style="color: #9CCBA8;">would
you please</span> <span style="color: #F3D56E;">move</span>
<span style="color: #9AB3E1;">8 / 2 meters</span><span style="color: #9CCBA8;">,
and then</span> <span style="color: #F3D56E;">rotate</span>
<span style="color: #9AB3E1;">90\*5
degrees</span><span style="color: #D27FA6;">.</span>

```
mov 4
rot 450
```

## Dependencies

- [GNU M4](https://www.gnu.org/software/m4/) v1.4.x
- [argp](https://www.gnu.org/software/libc/manual/html_node/Argp.html)
- [Flex](https://github.com/westes/flex) v2.x
- [Bison](https://www.gnu.org/software/bison/) v3.x

ROBINS uses `flex` for lexical analysis and `bison` for syntactical analysis.
GNU M4 is used to generate code for the user configured language.

Example installation on Ubuntu:

```bash
apt update
# libbison-dev is required for linking liby.a
apt install flex bison libbison-dev m4
```

## Usage

Configure the language parsed by ROBINS in `config/config.m4`, each
configuration option contains documentation on it's usage in the default config
file.

> While the M4 used the configuration is minimal, you can refer to the
> [GNU guide on M4 syntax](https://www.gnu.org/software/m4/manual/m4.html#Syntax)
> if you need help or want to explore more features.

Read the [wiki](https://github.com/sebascert/robins/wiki/Configurations) for the
full capabilities and limitations of languages parsed by ROBINS. The default
config file contains a simple usage of ROBINS for the parsing of a language with
this instructions:

```bash
make
```

Run the `--help` flag to list the available command line options:

```bash
./build/robins --help
```

ROBINS test:

```bash
./build/robins -i <<EOF
// Instructions may have have robot capitalized or not
robot please move 4 mtr.
Robot rotate 180 deg.
// Instructions may have polite words preceding them or not
robot please rotate 180 deg, then move 10 mtr.
// Degrees must be a multiple of 90
Robot rotate 89 deg.
EOF
```

## Contributing

Please read the [contributing guide](/CONTRIBUTING.md) in order to do so.
