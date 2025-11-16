
# CMakeLists.txt
cmake_minimum_required(VERSION 3.15)
project(dictionary C)

set(CMAKE_C_STANDARD 11)

# Create an executable (or multiple, if needed)
add_executable(dictionary
    variance.c
    hash.c
)
