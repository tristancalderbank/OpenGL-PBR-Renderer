add_executable(Ibl
        cubemapframebuffer.cpp
        main.cpp
        hdricube.cpp
        )

target_link_libraries(Ibl
        PRIVATE
        Engine
        )