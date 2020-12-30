cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

# Chakra helper library
project(CHEL)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

file(GLOB_RECURSE SRC ${SOURCE_DIR}/*.cpp)

add_library(CHEL ${SRC})
target_include_directories(CHEL PUBLIC ${SOURCE_DIR})
target_include_directories(CHEL PUBLIC ${INCLUDE_DIR})

target_precompile_headers(CHEL PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/CHEL/pch.h)

# Add libraries
target_link_libraries(CHEL PUBLIC chakracore)
target_link_libraries(CHEL PRIVATE fmt::fmt-header-only)