add_subdirectory(system)

add_library(runtime INTERFACE)

target_link_libraries(runtime INTERFACE system)

target_include_directories(runtime
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)