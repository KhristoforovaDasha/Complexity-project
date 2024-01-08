cmake_minimum_required(VERSION 3.25)
project(ComplexityRPP)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        benchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main
)
FetchContent_MakeAvailable(benchmark)

add_executable(performance test.cpp rpp.h)
target_link_libraries(performance benchmark_main benchmark)