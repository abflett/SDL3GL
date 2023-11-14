file(GLOB_RECURSE libsrc *.hpp *.cpp)

add_library(graphics ${libsrc})

target_link_libraries(graphics
    PUBLIC external
)

target_include_directories(graphics
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)