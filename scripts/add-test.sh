#!/bin/bash

set -euo pipefail

usage() {
    cat <<EOF
Usage: add-test.sh <testname>

Arguments:
  <testname>    Test name to create

Creates a new test in tests/<testname>
Each test consists of a directory test/ with:
  - run.sh:     Shell script with the execution of the test
                defaults to executing with input and writes
                to output.
  - config:     Robins config file, defaults to
                config/config.m4.
  - input:      Input file passed to the binary by path.
  - output:     Generated file with the output of the last
                test execution (if used by run.sh).
  - expected:   File with the expected result.

<testname>/run.sh is sourced from test.sh, if the script
returns non zero code the test is marked as failed and the
output and expected files are not compared. The following
environment variables are provided to run.sh:
  - test_bin:   Path to the binary of the
  - test_dir:   Path to test dir.
  - test_in:    Path to test input file.
  - test_out:   Path to output file to be compared with
                expected, this file is emptied prior to the
                execution of run.sh. Unless expected is
                missing, it will still be compared to it.
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
cat <<'EOF' > "$tests_dir/$testname/run.sh"
./$test_bin "$test_in" --output="$test_out" 2>/dev/null
EOF
cp "$default_config_dir/config.m4" "$tests_dir/$testname/"
touch "$tests_dir/$testname/input"
touch "$tests_dir/$testname/expected"

echo "Created $testname in '$tests_dir/$testname'" >&2
