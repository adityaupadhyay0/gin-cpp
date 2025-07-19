cmake_minimum_required(VERSION 3.16)
project(MyCppWebFw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-28-0 # Or any other recent tag
)

FetchContent_MakeAvailable(asio)

add_library(mycppwebfw INTERFACE)

target_include_directories(mycppwebfw INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${asio_SOURCE_DIR}/asio/include"
)

add_subdirectory(examples)
add_subdirectory(tests)
add_subdirectory(benchmarks)
