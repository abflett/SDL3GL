add_subdirectory(sdl)
add_subdirectory(sdl_image)
add_subdirectory(glad)
add_subdirectory(glm)

add_library(external INTERFACE)

target_link_libraries(external
    INTERFACE SDL3::SDL3
    INTERFACE SDL3_image::SDL3_image
    INTERFACE opengl32
    INTERFACE glm::glm
    INTERFACE glad
)