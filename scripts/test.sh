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

    test_bin=build/robins
    test_run="$test_dir/run.sh"
    test_in="$test_dir/input"
    test_out="$test_dir/output"
    test_expected="$test_dir/expected"

    [ ! -f "$test_run" ] && {
        echo "Skipping: $testname: missing run.sh script"
        continue
    } >&2
    [ ! -f "$test_dir/config.m4" ] && {
        echo "Skipping: $testname: missing robins config.m4 file"
        continue
    } >&2

    make config_dir="$test_dir" >&2

    echo -e "TEST $testname:\n" >&2

    source "$test_run" || {
        echo "Test $testname FAILED: run.sh exited with non-zero status"
        failed=$((failed+1))
        continue
    } >&2

    [ ! -r "$test_expected" ] && {
        echo "TEST $testname PASSED"
        continue
    } >&2

    if diff=$(diff --side-by-side <(nl "$test_expected") <(nl "$test_out")); then
        echo "TEST $testname PASSED" >&2
    else
        {
            echo "TEST $testname FAILED: output differs from expected"
            echo "  DIFF: $test_in - $test_out"
            echo "$diff"
            # input=$(grep -v '^//' <"$test_in")
            # lines=$(echo "$input" | wc -l)
            # paste <(seq 1 "$lines") <(echo "$input") <(echo "$diff") | column -s $'\t' -t
        } >&2

        failed=$((failed+1))
    fi
done

exit $failed
