cmake_minimum_required(VERSION 2.6)
project(data-structures-unit-tests)

file(GLOB SRCS *.cpp)
ADD_EXECUTABLE(testunit ${SRCS})
TARGET_LINK_LIBRARIES(
    testunit

    liblist

    libgtest
    libgmock
)

add_test(
    NAME testunit
    COMMAND testunit
)
