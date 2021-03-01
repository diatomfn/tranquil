cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(tranquil)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

file(GLOB_RECURSE SRC ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h)

add_library(tranquil ${SRC})
target_include_directories(tranquil PUBLIC ${SOURCE_DIR})
target_include_directories(tranquil PUBLIC ${INCLUDE_DIR})

target_precompile_headers(tranquil PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src/tranquil/pch.h)

# Add libraries
target_link_libraries(tranquil PUBLIC chakracore)