add_library(system Engine.cpp Engine.h)

target_link_libraries(system PRIVATE external)

target_include_directories(system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})