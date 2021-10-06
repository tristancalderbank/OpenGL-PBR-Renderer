cmake_minimum_required(VERSION 3.15)

project(NanoEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(dependencies)
add_subdirectory(engine)
add_subdirectory(ibl)
add_subdirectory(sample)