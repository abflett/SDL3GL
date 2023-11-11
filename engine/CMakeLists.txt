add_subdirectory(system)

add_library(engine INTERFACE)

target_link_libraries(engine INTERFACE system)

target_include_directories(engine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})