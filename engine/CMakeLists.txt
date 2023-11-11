add_subdirectory(system)

add_library(engine INTERFACE)

target_link_libraries(engine INTERFACE system)