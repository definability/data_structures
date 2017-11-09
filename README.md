# Data structures

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/e7d30b3feb73477c8fc5219677042ca5)](https://www.codacy.com/app/char-lie/data_structures?utm_source=github.com&utm_medium=referral&utm_content=char-lie/data_structures&utm_campaign=badger)

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
cmake .. && cmake --build .
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
watch --color "cmake .. && cmake --build . && GTEST_COLOR=1 ctest -V | tail"
```
