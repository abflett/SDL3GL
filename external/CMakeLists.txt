add_subdirectory(sdl)
add_subdirectory(sdl_image)
add_subdirectory(glad)
add_subdirectory(imgui)
add_subdirectory(glm)
add_subdirectory(entt)

add_library(external INTERFACE)

# Check if the platform is Windows or Linux
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(external
        INTERFACE SDL3::SDL3
        INTERFACE SDL3_image::SDL3_image
        INTERFACE opengl32
        INTERFACE glm::glm
        INTERFACE glad
        INTERFACE imgui
        INTERFACE EnTT::EnTT
    )
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(external
        INTERFACE SDL3::SDL3
        INTERFACE SDL3_image::SDL3_image
        INTERFACE GL
        INTERFACE glm::glm
        INTERFACE glad
        INTERFACE imgui
        INTERFACE EnTT::EnTT
    )
endif()
