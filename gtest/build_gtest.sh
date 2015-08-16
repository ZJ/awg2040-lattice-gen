#!/usr/bin/env bash

## Script to download and compile gtest

# If we're not on Travis, pretend we are
if test "true" = "$TRAVIS"
then
	echo -e "\e[34mWe're doing a build on TravisCI\e[0m"
else
	echo -e "\e[34mWe're not on TravisCI, setting up TRAVIS_BUILD_DIR\e[0m"
	TRAVIS_BUILD_DIR="$(pwd)"
fi


# Check if we're doing stuff with MingGW
if test -n "$HOST"
then
	GTEST_HOST="$HOST"
	echo -e "\e[33m(\e[31;1m#\e[0;33m^.^\e[31;1m#\e[0;33m) Cross-compilation currently unsupported by this project.\e[0m"
	exit 1
else
	GTEST_HOST=native
fi

# Where we expect to find/put things.
OUT_DIR="${TRAVIS_BUILD_DIR}/gtest/builds/${GTEST_HOST}"
LIB_DIR="${OUT_DIR}/lib"
SRC_DIR="${TRAVIS_BUILD_DIR}/gtest/src"

mkdir -p $OUT_DIR
mkdir -p $LIB_DIR
mkdir -p $SRC_DIR

# Move gtest to where we expect it
cp -pur /usr/src/gtest/* "$SRC_DIR"
cd $SRC_DIR

# Build gtest
echo -e "\e[34mBuilding via cmake\e[0m"
if cmake . && make
then
	echo -en ""
else
	echo -e "\e[31m(-_-* ) \e[1mBuilding gtest failed.\e[0m"
	cd ${TRAVIS_BUILD_DIR}
	exit 1
fi


# Copy build products/headers to final locations
cp -put "$LIB_DIR" "${SRC_DIR}/libgtest.a" "${SRC_DIR}/libgtest_main.a"

# Back to the build directory
cd ${TRAVIS_BUILD_DIR}
echo -e "\e[32m(^-^) gtest build successful!\e[0m"
