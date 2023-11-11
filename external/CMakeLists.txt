add_subdirectory(sdl)

add_library(external INTERFACE)

target_link_libraries(external INTERFACE SDL3::SDL3)