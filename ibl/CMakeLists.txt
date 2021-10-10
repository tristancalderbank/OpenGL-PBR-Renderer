add_executable(Ibl
        diffuseirradiancecube.cpp
        cubemapcube.cpp
        cubemapframebuffer.cpp
        main.cpp
        hdricube.cpp
        )

target_link_libraries(Ibl
        PRIVATE
        Engine
        )