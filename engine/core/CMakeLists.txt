add_subdirectory(common)
add_subdirectory(graphics)
add_subdirectory(input)
add_subdirectory(resources)


add_library(core INTERFACE)

target_link_libraries(core
    INTERFACE graphics
)

target_include_directories(core
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)