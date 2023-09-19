cmake_minimum_required(VERSION 3.0)
project(libjson VERSION 1.0)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 23)

if (BUILD_TESTING)
    enable_testing()
endif()

if (BUILD_TESTING)
    add_subdirectory(tests)
endif()
