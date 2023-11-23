# file(GLOB_RECURSE libsrc *.hpp *.cpp)

# add_library(imgui ${libsrc})


add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_demo.cpp
    imgui/backends/imgui_impl_sdl3.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(imgui PUBLIC SDL3::SDL3)

target_include_directories(imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)
