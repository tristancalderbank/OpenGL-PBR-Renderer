add_executable(Ibl
        main.cpp
        hdricube.cpp
        )

target_link_libraries(Ibl
        PRIVATE
        Engine
        )