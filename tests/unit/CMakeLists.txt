cmake_minimum_required(VERSION 2.6)
project(data-structures-unit-tests)

file(GLOB SRCS *.cpp)
add_executable(testunit ${SRCS})
target_link_libraries(
    testunit

    liblist

    libgtest
    libgmock
)

add_test(
    NAME testunit
    COMMAND testunit
)
