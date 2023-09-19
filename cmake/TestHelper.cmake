find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

macro(just_add_test TEST_PREFIX TEST_FILE TEST_LIBRARY)
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    set(TEST_NAME "${TEST_PREFIX}_${TEST_NAME}")
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_link_libraries(${TEST_NAME} ${TEST_LIBRARY} ${GTEST_BOTH_LIBRARIES} ${GMOCK_BOTH_LIBRARIES})
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME} WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endmacro()
