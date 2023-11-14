add_subdirectory(graphics)

add_library(core INTERFACE)

target_link_libraries(core
    INTERFACE graphics
)

target_include_directories(core
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)