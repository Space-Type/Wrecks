#!/bin/bash

REPO_DIR="$(dirname $(realpath $0))"
BUILD_DIR="$REPO_DIR/build"

if ! [ -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
    cmake -DCMAKE_BUILD_TYPE=Debug -S "$REPO_DIR" -B "$BUILD_DIR"
fi

cmake --build "$BUILD_DIR"
ctest --test-dir "$BUILD_DIR"
gcovr -r "$REPO_DIR" --exclude "$BUILD_DIR"
