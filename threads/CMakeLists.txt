cmake_minimum_required(VERSION 3.24)
project(exam_preparing)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=thread -g)
add_link_options(-fsanitize=thread)

add_executable(threads threads.cpp)
