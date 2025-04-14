#!/bin/bash

set -euo pipefail
trap 'echo "failed:$LINENO \"$BASH_COMMAND\""' ERR

usage() {
    echo "Usage: $0 <tests_dir> <testname>"
    echo
    echo "Arguments:"
    echo "  <tests_dir>   Directory containing test input/output files"
    echo "  <testname>    Test name to create"
    echo
    echo "Returns: succesfull creation"
    echo
    echo "Creates a new test in test_dir/<testname>"
    echo
    echo "Example:"
    echo "  $0 arithmetic"
}

case "$1" in
    -h|--help)
        usage
        exit 0
        ;;
esac

if [ $# -ne 2 ]; then
    usage
    exit 1
fi

tests_dir="$1"
testname="$2"

mkdir -p "$tests_dir/$testname"
touch  "$tests_dir/$testname/input" "$tests_dir/$testname/expected"

echo "Created $testname in '$tests_dir/$testname'"
