add_library(Stb stb_image.cpp)

target_include_directories(Stb
        PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}"
        )