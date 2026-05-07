cmake_minimum_required(VERSION 3.16)
project(CVMPlusPlus VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags for macOS
if(APPLE)
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.cpp
    src/token.cpp
    src/lexer.cpp
    src/ast.cpp
    src/parser.cpp
    src/opcodes.cpp
    src/compiler.cpp
    src/vm.cpp
    src/repl.cpp
)

# Create executable
add_executable(cvm++ ${SOURCES})

# Installation
install(TARGETS cvm++ DESTINATION bin)