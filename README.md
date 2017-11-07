# Data structures

![Build Status](https://api.travis-ci.org/char-lie/data_structures.svg?branch=master)


Data structures playground.

# Development

## Build

It's recommended to build the project in separate `build` folder

```bash
mkdir -p build && cd build
```

Project uses [CMake](https://cmake.org/) and it's very easy in use.
Just execute following code in the `build` directory

```bash
cmake .. && make
```

## Unit tests

Project uses [GTest](https://github.com/google/googletest) framework.
To run tests you can execute unit tests from `build` directory of built project

```bash
make test
```

You can use `ctest` as well.
For verbose output with colors use

```bash
GTEST_COLOR=1 ctest -V
```

## Autobuild

Go to your `build` directory and execute `watch`

```bash
watch --color "cmake .. && make && GTEST_COLOR=1 ctest -V | tail"
```
