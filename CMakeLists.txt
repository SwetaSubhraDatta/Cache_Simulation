cmake_minimum_required(VERSION 3.20)
project(563s21_p3_cpp)

set(CMAKE_CXX_STANDARD 11)

set(
        sim_cache_src cache.cc MatMul.cc
)
set(
        sim_cache_hdr cache.h
        )

add_library(
        sim_cache STATIC
        ${sim_cache_src}
        ${sim_cache_hdr}
)
target_include_directories(sim_cache PUBLIC .)

add_executable(563s21_p3_cpp cache.cc)

add_subdirectory(testcases)
