cmake_minimum_required(VERSION 3.15)

project(TinyEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(TinyEngine main.cpp)

target_include_directories(TinyEngine PRIVATE lib)

add_subdirectory(lib)

target_link_libraries(TinyEngine ${CMAKE_SOURCE_DIR}/lib/glfw/glfw3.lib)
target_link_libraries(TinyEngine ${CMAKE_SOURCE_DIR}/lib/assimp/assimp-vc142-mt.lib)