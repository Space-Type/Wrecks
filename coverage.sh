#!/bin/bash

REPO_DIR="$(dirname $(realpath $0))"
BUILD_DIR="$REPO_DIR/build"

ctest --test-dir "$BUILD_DIR"
gcovr -r "$REPO_DIR" --exclude "$BUILD_DIR"
