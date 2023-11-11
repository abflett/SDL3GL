add_subdirectory(sdl)
add_subdirectory(glad)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/glad/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/glad/glad/cmake)

add_library(external INTERFACE)

target_link_libraries(external INTERFACE SDL3::SDL3 opengl32 glad)