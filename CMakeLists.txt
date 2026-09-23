cmake_minimum_required(VERSION 3.12)
project(spsc-ring-buffer LANGUAGES CXX)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT MSVC)
    add_compile_options(-O3 -march=native)
endif()

# Initial Main Script
file(GLOB MAIN "main.cpp")

# Create initial main script
add_executable(main ${MAIN})
