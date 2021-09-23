cmake_minimum_required(VERSION 3.15)

project(TinyEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(TinyEngine main.cpp)

target_include_directories(TinyEngine PRIVATE lib)

add_subdirectory(lib)

# link libraries
if (WIN32)
    target_link_libraries(TinyEngine ${CMAKE_SOURCE_DIR}/lib/GLFW/glfw3.lib)
    target_link_libraries(TinyEngine ${CMAKE_SOURCE_DIR}/lib/assimp/assimp-vc142-mt.lib)
elseif(APPLE)
    target_link_libraries(TinyEngine ${CMAKE_SOURCE_DIR}/lib/GLFW/libglfw.3.dylib)
    target_link_libraries(TinyEngine ${CMAKE_SOURCE_DIR}/lib/assimp/libassimp.5.0.0.dylib)
    target_link_libraries(TinyEngine ${CMAKE_SOURCE_DIR}/lib/assimp/libIrrXML.dylib)
else()
    message(FATAL_ERROR "Unsupported OS")
endif()