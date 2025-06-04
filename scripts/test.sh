#!/bin/bash

set -euo pipefail

usage() {
    cat <<EOF
Usage: test.sh [tests]

Arguments:
  [tests]       List of tests to run, if empty run all tests

Returns: count of failed tests
EOF
}

if [ $# -ge 1 ]; then
    case "$1" in
        -h|--help)
            usage >&2
            exit 0
            ;;
    esac
fi

script_dir="$(cd "$(dirname "$0")/.." && pwd)"
cd "$script_dir" || {
    echo "Error: unable to enter script parent parent directory"
    echo "Error: dir: $script_dir"
    exit 1
} >&2

binary=build/robins
tests_dir=tests

{ [ ! -d "$tests_dir" ] || [ -z "$(ls -A "$tests_dir")" ]; } && {
    echo "No tests to run"
    exit 0
} >&2


if [ $# -eq 0 ]; then
    while IFS= read -r -d '' dir; do
        testname="$(basename "$dir")"
        tests+=("$testname")
    done < <(find "$tests_dir" -mindepth 1 -maxdepth 1 -type d -print0)
else
    tests=("$@")
fi

for t in "${tests[@]}"; do
    [ ! -d "$tests_dir/$t" ] && {
        echo "Error: test '$t' does not exist (expected test/$t)"
        exit 1
    } >&2
done

echo "Running tests..." >&2

failed=0
for testname in "${tests[@]}"; do
    test_dir="$tests_dir/$testname"

    test_input="$test_dir/input"
    test_expected="$test_dir/expected"

    [ ! -f "$test_dir/config.m4" ] || {
        echo "Skipping $testname: missing robins config file"
        continue
    } >&2
    [ ! -f "$test_input" ] || {
        echo "Skipping $testname: missing input file"
        continue
    } >&2
    [ ! -f "$test_expected" ]|| {
        echo "Skipping $testname: missing expected file"
        continue
    } >&2

    test_output="$test_dir/output"
    test_args=$(cat "$test_dir/args" 2>/dev/null || true)

    make config_dir="$test_dir"

    echo -n "Test $testname: "

    # execute binary and discard control output
    $binary $test_args "$test_input" "--output=$test_output" 2>/dev/null

    if diff=$(diff -y "$test_expected" "$test_output"); then
        echo "PASSED"
    else
        echo "FAILED"
        echo "  DIFF:"
        input=$(grep -v '^//' <"$test_input")
        lines=$(echo "$input" | wc -l)
        paste <(seq 1 "$lines") <(echo "$input") <(echo "$diff") | column -s $'\t' -t

        failed=$((failed+1))
    fi
done

exit $failed
