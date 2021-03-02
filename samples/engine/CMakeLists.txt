cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(tranquil_samples_engine)

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})

file(GLOB_RECURSE SRC ${SOURCE_DIR}/*.cpp ${SOURCE_DIR}/*.h)

add_executable(tranquil_samples_engine ${SRC})
target_include_directories(tranquil_samples_engine PUBLIC ${SOURCE_DIR})

target_link_libraries(tranquil_samples_engine PUBLIC tranquil)