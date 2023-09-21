cmake_minimum_required(VERSION 3.0)
project(libjson VERSION 1.0)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
