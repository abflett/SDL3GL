add_executable(demo main.cpp)

target_link_libraries(demo PUBLIC engine)

add_custom_command(
    TARGET demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:demo>/assets
)