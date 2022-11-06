#!/bin/sh

set -e
runpath=$(cd -P -- "$(dirname -- "$0")" && pwd -P)
SOURCE_DIR="." "$runpath/makeImage.sh"