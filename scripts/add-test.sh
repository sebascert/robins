#!/bin/bash

set -euo pipefail

usage() {
    cat <<EOF
Usage: add-test.sh <testname>

Arguments:
  <testname>    Test name to create

Creates a new test in tests/<testname>
Each test consists of a directory test/ with:
  - args:       file binary containing positional arguments.
  - config:     robins config file.
  - input:      input file passed to the binary by path.
  - expected:   file with the expected result.
  - output:     generated file with the output of the last
                test execution.
EOF
}

[[ $# -ne 1 ]] && {
    usage >&2
    exit 1
} >&2

case "$1" in
    -h|--help)
        usage
        exit 0
        ;;
esac

script_dir="$(cd "$(dirname "$0")/.." && pwd)"
cd "$script_dir" || {
    echo "Error: unable to enter script parent parent directory"
    echo "Error: dir: $script_dir"
    exit 1
} >&2

default_config_dir="config"
tests_dir="tests"
mkdir -p "$tests_dir"

testname="$1"
[ -d "$tests_dir/$testname" ] && {
    echo "test already exists"
    exit 1
} >&2

mkdir "$tests_dir/$testname"
touch "$tests_dir/$testname/args"
touch "$tests_dir/$testname/input"
touch "$tests_dir/$testname/expected"
cp "$default_config_dir/config.m4" "$tests_dir/$testname/"

echo "Created $testname in '$tests_dir/$testname'" >&2
