cmake_minimum_required(VERSION 3.15)

project(SDL3GL VERSION 0.0.1)


set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/Debug)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external)
add_subdirectory(engine)
add_subdirectory(demo)

# Print CMAKE_BUILD_TYPE
if(CMAKE_BUILD_TYPE)
    message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
else()
    message(WARNING "Build type not explicitly set.")
endif()

# Determine the build directory based on CMAKE_BUILD_TYPE or CMAKE_CONFIGURATION_TYPES
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
else()
    set(BUILD_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
endif()

# Copy assets to build directory
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${BUILD_DIR}/assets
)

# Add a dependency so that the assets are copied before building the executable
add_dependencies(demo copy_assets)