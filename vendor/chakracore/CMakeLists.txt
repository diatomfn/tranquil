cmake_minimum_required(VERSION 3.16)
project(chakracore)

add_library(chakracore SHARED IMPORTED GLOBAL)

set_target_properties(chakracore
        PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
        IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libChakraCore.so
        )

target_include_directories(chakracore INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)