cmake_minimum_required(VERSION 3.10)
project(MyCppWebFw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(mycppwebfw src/core/thread_pool.cpp)
target_include_directories(mycppwebfw PUBLIC include)

include(GoogleTest)
enable_testing()

add_executable(thread_pool_test tests/core/thread_pool_test.cpp)
target_link_libraries(thread_pool_test mycppwebfw gtest_main)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_test(NAME thread_pool_test COMMAND thread_pool_test)
