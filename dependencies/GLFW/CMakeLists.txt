add_library(GLFW INTERFACE)

target_include_directories(GLFW
        INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}"
        )

if (WIN32)
    target_link_libraries(GLFW INTERFACE "${CMAKE_CURRENT_LIST_DIR}/glfw3.lib")
elseif(APPLE)
    target_link_libraries(GLFW INTERFACE "${CMAKE_CURRENT_LIST_DIR}/libglfw.3.dylib")
else()
    message(FATAL_ERROR "Unsupported OS")
endif()