# Get all test source files
file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/rate_limiter_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/cors_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/logger_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/compression_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/request_id_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/request_id_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/auth_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/auth_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/middleware_tests/session_test.cpp"
)


# Create a test executable
add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests PRIVATE mycppwebfw devtools GTest::gtest_main zlib)
target_include_directories(run_tests PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${CMAKE_CURRENT_SOURCE_DIR}/../src"
)

# Add the test to CTest
add_test(NAME run_tests COMMAND run_tests)
