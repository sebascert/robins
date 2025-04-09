#!/bin/bash

build=$1
test_out=$2

echo "Running test..."

test(){
    expr="$1"
    expected="$2"
    echo "$expr" | "./$build" > "$test_out"
    grep -q "$expected" "$test_out" ||
    echo "❌ Test failed! expected:$expected received:$(cat "$test_out")"
    return 1
}

passed=1
test "(1+2)+3" "= 6" || passed=0
test "1+(2+3)" "= 6" || passed=0
if [ $passed -eq 1 ]; then
    echo "✅ associativity of addition passed!"
fi

passed=1
test "(2*3)*4" "= 24" || passed=0
test "2*(3*4)" "= 24" || passed=0
if [ $passed -eq 1 ]; then
    echo "✅ associativity of multiplication passed!"
fi

passed=1
test "2*(3+4)" "= 14" || passed=0
test "2*3 + 2*4" "= 14" || passed=0
if [ $passed -eq 1 ]; then
    echo "✅ distributivity passed!"
fi

passed=1
test "1+2*3" "= 7" || passed=0
test "2*3+1" "= 7" || passed=0
if [ $passed -eq 1 ]; then
    echo "✅ operator precedence passed!"
fi

passed=1
test "5+0" "= 5" || passed=0
test "0+5" "= 5" || passed=0
test "7*1" "= 7" || passed=0
test "1*7" "= 7" || passed=0
if [ $passed -eq 1 ]; then
    echo "✅ identity elements passed!"
fi
