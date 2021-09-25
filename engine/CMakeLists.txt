add_library(Engine
        src/camera.cpp
        src/framebuffer.cpp
        src/fullscreenquad.cpp
        src/mesh.cpp
        src/model.cpp
        src/shader.cpp
        src/skybox.cpp
        )

target_include_directories(Engine
        PUBLIC
            "${CMAKE_CURRENT_LIST_DIR}/include"
        )

target_link_libraries(Engine
        PUBLIC
        GLFW
        Glad
        Stb
        Assimp
        Imgui
        )