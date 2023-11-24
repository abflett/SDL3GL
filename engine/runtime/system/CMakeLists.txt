file(GLOB_RECURSE libsrc *.hpp *.cpp)

add_library(system ${libsrc})

target_link_libraries(system
    PUBLIC external
    PUBLIC core
    PUBLIC input
)

target_include_directories(system
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)