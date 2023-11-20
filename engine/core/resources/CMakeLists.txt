file(GLOB_RECURSE libsrc *.hpp *.cpp)

add_library(resources ${libsrc})

target_link_libraries(resources
    PUBLIC external
)

target_include_directories(resources
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)