add_library(imgui STATIC imgui/imgui.cpp imgui/imgui.h)

target_include_directories(imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends
)
