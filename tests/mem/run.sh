# --show-leak-kinds=reachable is not used as there are many still reachable
# errors from non user code
valgrind_args=(
    "--leak-check=full"
    "--show-leak-kinds=definite,indirect,possible"
    "--error-exitcode=1"
)
valgrind "${valgrind_args[@]}" \
    "$test_bin" "$test_in" --output="$test_out" || return 1
valgrind "${valgrind_args[@]}" \
    "$test_bin" "$test_in" --output="$test_out" --graph || return 1
