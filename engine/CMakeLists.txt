add_subdirectory(core)
add_subdirectory(runtime)

add_library(engine INTERFACE)

target_link_libraries(engine INTERFACE runtime)

target_include_directories(engine
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)