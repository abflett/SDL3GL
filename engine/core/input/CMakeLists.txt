file(GLOB_RECURSE libsrc *.hpp *.cpp)

add_library(input ${libsrc})

target_link_libraries(input
    PUBLIC external
)

target_include_directories(input
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)