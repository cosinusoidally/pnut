#!/bin/sh
#
# Run tests for pnut using different backends
# Usage: ./run-tests.sh <backend> --shell <shell> --match <pattern> --bootstrap
# The backend can be one of the following: sh, i386_linux, x86_64_linux, x86_64_mac
# The --shell flag is used to specify the shell to use for running tests with the sh backend
# The --match flag is used to run tests that match the given pattern, useful for re-running failed tests
# The --bootstrap flag compiles the tests using pnut compiled with pnut, useful for catching bootstrap errors

trap "exit 1" INT

fail() { echo "$1"; exit $2; }

if [ $# -lt 1 ]; then
  fail "Usage: $0 <backend> -m pattern --bootstrap" 1
fi

# Parse the arguments
backend=$1; shift
bootstrap=0
shell="$SHELL" # Use current shell as the default
pattern=".*"
while [ $# -gt 0 ]; do
  case $1 in
    --shell) shell="$2";      shift 2;;
    --match) pattern="$2";    shift 2;;
    --bootstrap) bootstrap=1; shift 1;;
    *) echo "Unknown option: $1"; exit 1;;
  esac
done

# Determine the file extension based on the backend
case "$backend" in
  sh)
    ext="exe" # The extension doesn't matter for sh
    PNUT_EXE_OPTIONS="-Dsh -DRT_NO_INIT_GLOBALS"
    ;;
  i386_linux | x86_64_linux | x86_64_mac)
    ext="exe"
    PNUT_EXE_OPTIONS="-Dtarget_$backend"
    ;;
  *)
    echo "Unknown backend: $backend"
    exit 1
    ;;
esac

# Compile pnut, either using gcc or with pnut itself
# pnut_comp is set to the compiled pnut executable
compile_pnut() {
  pnut_source="pnut.c"
  pnut_exe="./tests/pnut-by-gcc.exe"
  pnut_exe_backend="./tests/pnut.$ext"

  echo "Compiling $pnut_source with $backend backend..."
  gcc "$pnut_source" $PNUT_EXE_OPTIONS -o "$pnut_exe" 2> /dev/null || fail "Error: Failed to compile $pnut_source with $backend"
  if [ "$bootstrap" -eq 1 ]; then
    "$pnut_exe" $PNUT_EXE_OPTIONS "$pnut_source" > "$pnut_exe_backend" || fail "Error: Failed to compile $pnut_source with $pnut_exe (bootstrap)"
    chmod +x "$pnut_exe_backend"
    pnut_comp="$pnut_exe_backend"
  else
    pnut_comp="$pnut_exe"
  fi
}

# Some tests specify command line arguments in the source file
# This function extracts the arguments from the source file
# To specify arguments, add a comment in the source file like this:
# // args: arg1 arg2 arg3
test_args() {
  echo `sed -n -e "/\/\/ args:/p" "$1" | sed -e "s/^\/\/ args://" |  tr '\n' ',' | sed -e 's/,$//'`
}

execute_test() { # executable: $1, args: $2 ...
  if [ "$backend" = "sh" ]; then
    # Default to bash for sh backend
    # Use a 5s timeout to prevent infinite loops
    timeout 5 bash "./$1" $2
  else
    "./$1" $2
  fi
}

compile_test() { # c_file: $1
  # 5s timeout to prevent infinite loops in pnut
  timeout 5 "$pnut_comp" "$1"
}

run_test() { # file_to_test: $1
  file="$1"
  filename=$(basename "$file" .c) # Get the filename without extension
  dir=$(dirname "$file") # Get the directory of the test file

  golden_file="$dir/$filename.golden"

  # Print file name before generating golden file so we know it's getting processed
  printf "$file: "

  # Generate golden file if it doesn't exist
  if [ ! -f "$golden_file" ]; then
    compile_test "$file" > "$dir/$filename.$ext" 2> "$dir/$filename.err"
    if [ $? -eq 0 ]; then
      chmod +x "$dir/$filename.$ext"
      execute_test "$dir/$filename.$ext" "$(test_args $file)" > "$golden_file"
      echo "🟡 Golden file generated by pnut"
    else
      echo "❌ Failed to compile with pnut"
    fi
    return 1
  fi

  # Compile the test file with pnut.exe
  compile_test "$file" > "$dir/$filename.$ext" 2> "$dir/$filename.err"

  if [ $? -eq 0 ]; then # If compilation was successful
    chmod +x "$dir/$filename.$ext"
    execute_test "$dir/$filename.$ext" "$(test_args $file)" > "$dir/$filename.output" 2> "$dir/$filename.err"
    if [ $? -eq 0 ]; then # If the executable ran successfully
        diff_out=$(diff "$dir/$filename.output" "$dir/$filename.golden")
        if [ $? -eq 0 ]; then # If the output matches the golden file
          echo "✅ Test passed"
          return 0
        else
          echo "❌ Test failed"
          echo "diff (output vs expected)"
          echo "$diff_out"
          return 1
        fi
      else
        echo "❌ Failed to run: $(cat "$dir/$filename.err")"
        return 1
      fi
  else
    echo "❌ Failed to compile with pnut: $(cat "$dir/$filename.err")"
    return 1
  fi
}

run_tests_in_folder() {
  folder="$1"
  for file in $(find "$folder" -type f -name "*.c" | sort | grep -E "$pattern"); do
    if run_test "$file"; then
      passed_tests="$passed_tests\n$file"
    else
      failed_tests="$failed_tests\n$file"
    fi
  done
}

# Function to run tests
run_tests() {
  passed_tests="" # List of passed tests separated by newline
  failed_tests="" # List of failed tests separated by newline

  echo "Running tests..."

  run_tests_in_folder "tests/_all"
  if [ "$backend" = "sh" ]; then
    run_tests_in_folder "tests/_sh"
  else # Run all tests for other backends
    run_tests_in_folder "tests/_exe"
  fi

  echo "Summary:"
  echo "===================="
  echo "Passed: $(printf "$passed_tests" | wc -l)"
  echo "Failed: $(printf "$failed_tests" | wc -l)"

  if [ -n "$failed_tests" ]; then
    for file in $(printf "$failed_tests"); do
      printf " - %s\n" "$file"
    done
    exit 1
  else
    exit 0
  fi
}

compile_pnut
run_tests "$pattern"
