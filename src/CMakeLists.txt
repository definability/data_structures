cmake_minimum_required(VERSION 2.6)
project(data-structures)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(data_structures_srcs main.cpp)
add_library(libsrc STATIC ${data_structures_srcs})

target_include_directories(
    libsrc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set(list_src list.cpp)
add_library(liblist STATIC ${list_src})
target_include_directories(
    liblist PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
