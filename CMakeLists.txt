cmake_minimum_required(VERSION 3.12)
project(spsc-ring-buffer LANGUAGES CXX)

# Require C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Gather files for bench.
file(GLOB BENCH "bench.cpp")

# Fetch Google Benchmark Library.
include(FetchContent)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.1
)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(benchmark)

# Create bench executable
add_executable(bench ${BENCH})
target_link_libraries(bench PRIVATE benchmark::benchmark)

# Bench Executable with Thread Sanitizer.
add_executable(bench-debug ${BENCH})
target_link_libraries(bench-debug PRIVATE benchmark::benchmark)

if(NOT MSVC)
    target_compile_options(bench PRIVATE -O3 -march=native)
    target_compile_options(bench-debug PRIVATE -fsanitize=thread -g -O1)
    target_link_options(bench-debug PRIVATE -fsanitize=thread)
endif()




