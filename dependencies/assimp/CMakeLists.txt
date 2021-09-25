add_library(Assimp INTERFACE)

target_include_directories(Assimp
        INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}"
        )

if (WIN32)
    target_link_libraries(Assimp INTERFACE "${CMAKE_CURRENT_LIST_DIR}/assimp-vc142-mt.lib")
elseif(APPLE)
    target_link_libraries(Assimp INTERFACE "${CMAKE_CURRENT_LIST_DIR}/libassimp.5.0.0.dylib")
    target_link_libraries(Assimp INTERFACE "${CMAKE_CURRENT_LIST_DIR}/libIrrXML.dylib")
else()
    message(FATAL_ERROR "Unsupported OS")
endif()