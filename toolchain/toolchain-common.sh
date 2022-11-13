#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
TARGET="i686-pc-avuos"
PREFIX="$DIR/tools"
BUILD="$DIR/build"
EDIT="$DIR/edit"
SYSROOT="$DIR/sysroot"
NUM_JOBS=$(nproc)

BINUTILS_VERSION="2.34"
BINUTILS_FILE="binutils-$BINUTILS_VERSION"
BINUTILS_URL="https://ftp.gnu.org/gnu/binutils/$BINUTILS_FILE.tar.gz"

GCC_VERSION="9.3.0"
GCC_FILE="gcc-$GCC_VERSION"
GCC_URL="https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/$GCC_FILE.tar.gz"

NEWLIB_VERSION="3.3.0"
NEWLIB_FILE="newlib-$NEWLIB_VERSION"
NEWLIB_URL="ftp://sourceware.org/pub/newlib/$NEWLIB_FILE.tar.gz"

download-binutils () {
	if [ ! -d "$BINUTILS_FILE" ]; then
	printf "Downloading binutils %s...\n" "$BINUTILS_VERSION"
	curl "$BINUTILS_URL" > "$BINUTILS_FILE.tar.gz" || exit 1
	printf "Extracting binutils...\n"
	tar -xzf "$BINUTILS_FILE.tar.gz" || exit 1
	rm "$BINUTILS_FILE.tar.gz"

	cd "$BINUTILS_FILE" || exit 1
	printf "Patching binutils...\n"
	if [ "$1" == "use-git" ]; then
		git init . > /dev/null || exit 1
		git add -A > /dev/null || exit 1
		git commit -m "First commit" > /dev/null || exit 1
		git apply "$DIR/binutils-$BINUTILS_VERSION.patch" > /dev/null || exit 1
	else
		patch -p1 < "$DIR/binutils-$BINUTILS_VERSION.patch" > /dev/null || exit 1
	fi
	printf "binutils patched!\n"
	cd ..
	else
	printf "binutils already downloaded.\n"
	fi
}

download-gcc () {
	if [ ! -d "$GCC_FILE" ]; then
	printf "Downloading gcc %s...\n" "$GCC_VERSION"
	curl "$GCC_URL" > "$GCC_FILE.tar.gz" || exit 1
	printf "Extracting gcc...\n"
	tar -xzf "$GCC_FILE.tar.gz" || exit 1
	rm "$GCC_FILE.tar.gz"

	cd "$GCC_FILE" || exit 1
	printf "Patching gcc...\n"
	if [ "$1" == "use-git" ]; then
		git init . > /dev/null || exit 1
		git add -A > /dev/null || exit 1
		git commit -m "First commit" > /dev/null || exit 1
		git apply "$DIR/gcc-$GCC_VERSION.patch" > /dev/null || exit 1
	else
		patch -p1 < "$DIR/gcc-$GCC_VERSION.patch" > /dev/null || exit 1
	fi
	printf "gcc patched!\n"
	cd ..
	else
	printf "gcc already downloaded.\n"
	fi
}

download-newlib () {
	if [ ! -d "$NEWLIB_FILE" ]; then
	printf "Downloading newlib %s...\n" "$NEWLIB_VERSION"
	curl "$NEWLIB_URL" > "$NEWLIB_FILE.tar.gz" || exit 1
	printf "Extracting newlib...\n"
	tar -xzf "$NEWLIB_FILE.tar.gz" || exit 1
	rm "$NEWLIB_FILE.tar.gz"

	cd "$NEWLIB_FILE" || exit 1
	printf "Patching newlib...\n"
	if [ "$1" == "use-git" ]; then
		git init . > /dev/null || exit 1
		git add -A > /dev/null || exit 1
		git commit -m "First commit" > /dev/null || exit 1
		git apply "$DIR/newlib-$NEWLIB_VERSION.patch" > /dev/null || exit 1
	else
		patch -p1 < "$DIR/newlib-$NEWLIB_VERSION.patch" > /dev/null || exit 1
	fi
	printf "newlib patched!\n"
	cd ..
	else
	printf "newlib already downloaded.\n"
	fi
}