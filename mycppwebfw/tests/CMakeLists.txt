# Get all test source files
file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
)

# Create a test executable
add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE mycppwebfw GTest::gtest_main)

# Add the test to CTest
add_test(NAME run_tests COMMAND run_tests)
