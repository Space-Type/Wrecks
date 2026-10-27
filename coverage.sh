#!/bin/bash

METRIC="$1"
REPO_DIR="$(dirname $(realpath $0))"
BUILD_DIR="$REPO_DIR/build"
TESTS_DIR="$REPO_DIR/tests"
COVRG_DIR="$REPO_DIR/test-coverage"

if [ "$METRIC" ]; then
    METRIC="--txt-metric $METRIC"
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
gcovr -r "$REPO_DIR" $METRIC --exclude "$TESTS_DIR" --exclude "$BUILD_DIR" --exclude-throw-branches --html-details "$COVRG_DIR/index.html"
