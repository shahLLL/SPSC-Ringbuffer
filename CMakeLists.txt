cmake_minimum_required(VERSION 3.12)
project(spsc-ring-buffer LANGUAGES CXX)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gather files for main.
file(GLOB MAIN "main.cpp")

# Create main executable
add_executable(main ${MAIN})

# Main Executable with Thread Sanitizer.
add_executable(main-debug ${MAIN})

if(NOT MSVC)
    target_compile_options(main PRIVATE -O3 -march=native)
    target_compile_options(main-debug PRIVATE -fsanitize=thread -g -O1)
    target_link_options(main-debug PRIVATE -fsanitize=thread)
endif()




