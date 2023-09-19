include(TestHelper)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach (test_file ${TEST_SOURCES})
    just_add_test(libjson ${test_file} "")
endforeach ()
