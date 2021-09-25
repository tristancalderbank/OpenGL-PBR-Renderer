add_library(Glad glad.c)

target_include_directories(Glad
        PUBLIC
        "${CMAKE_CURRENT_LIST_DIR}/.."
        )