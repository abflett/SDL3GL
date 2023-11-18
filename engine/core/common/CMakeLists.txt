file(GLOB_RECURSE libsrc *.hpp *.cpp)

add_library(common ${libsrc})

target_link_libraries(common
    PUBLIC external
)

target_include_directories(common
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)