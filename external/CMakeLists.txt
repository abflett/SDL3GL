add_subdirectory(sdl)
add_subdirectory(glad)
add_subdirectory(glm)

add_library(external INTERFACE)

target_link_libraries(external
    INTERFACE SDL3::SDL3
    INTERFACE opengl32
    INTERFACE glm::glm
    INTERFACE glad
)