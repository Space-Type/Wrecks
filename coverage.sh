#!/bin/bash

display_help() {
    cat << EOF
Usage: $0 [OPTIONS...]
Generate a coverage report for the project's unit tests.
Example: $0 -m branch

Options:
  -d, --html-details          generate a HTML coverage report with details
  -h, --help                  display this help message and exit
  -m, --metric line|branch|decision
                              choose the coverage metric
  --no-line-merge             disable line merging
EOF
}

# Config
REPO_DIR="$(dirname $(realpath $0))"
BUILD_DIR="$REPO_DIR/build"
TESTS_DIR="$REPO_DIR/tests"
COVRG_DIR="$REPO_DIR/test-coverage"

# Options
GCOVR_EXTRA_OPTS=""
NO_LINE_MERGE=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        -d|--html-details)
            GCOVR_EXTRA_OPTS="--html-details $COVRG_DIR/index.html $GCOVR_EXTRA_OPTS"
            shift
            ;;
        -h|--help)
            display_help
            exit 0
            ;;
        -m|--metric)
            if ! [ "$2" ] || [[ "$2" =~ ^- ]]; then
                echo "$1 requires an extra argument"
                exit 1
            fi
            GCOVR_EXTRA_OPTS="--txt-metric $2 $GCOVR_EXTRA_OPTS"
            shift 2
            ;;
        --no-line-merge)
            NO_LINE_MERGE="y"
            shift
            ;;
        -*)
            echo "Unknown option: $1" >&2
            exit 1
            ;;
        *)
            # Positional argument
            echo "Unexpected argument $1" >&2
            exit 1
            ;;
    esac
done

if [ -z "$NO_LINE_MERGE" ]; then
    GCOVR_EXTRA_OPTS="--merge-lines $GCOVR_EXTRA_OPTS"
fi

if ! [ -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

if ! [ -d "$COVRG_DIR" ]; then
    mkdir "$COVRG_DIR"
fi

cmake -DCMAKE_BUILD_TYPE=DebugTests -S "$REPO_DIR" -B "$BUILD_DIR"
cmake --build "$BUILD_DIR"
ctest --test-dir "$BUILD_DIR"
gcovr -r "$REPO_DIR" --exclude "$TESTS_DIR" --exclude "$BUILD_DIR" --exclude-throw-branches $GCOVR_EXTRA_OPTS
