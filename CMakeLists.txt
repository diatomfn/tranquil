cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

# Chakra helper library
project(tranquil_root)

# Dependency directory
set(VENDOR_DIR ${CMAKE_CURRENT_LIST_DIR}/vendor)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Build targets
add_subdirectory(tranquil)
add_subdirectory(sandbox)

# Samples
add_subdirectory(samples/engine)

# Register libraries
add_subdirectory(${VENDOR_DIR}/chakracore)