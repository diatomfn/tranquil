cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(tranquil_sandbox)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

file(GLOB_RECURSE SRC ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h)

add_executable(tranquil_sandbox ${SRC})
target_include_directories(tranquil_sandbox PUBLIC ${SOURCE_DIR})

target_link_libraries(tranquil_sandbox PUBLIC tranquil)