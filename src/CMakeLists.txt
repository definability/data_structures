cmake_minimum_required(VERSION 2.6)
project(data-structures)

set(list_src list.cpp)
add_library(liblist STATIC ${list_src})
target_include_directories(
    liblist PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
