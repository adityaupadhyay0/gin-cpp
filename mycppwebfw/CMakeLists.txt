cmake_minimum_required(VERSION 3.16)
project(MyCppWebFw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options for enabling/disabling features
option(ENABLE_TESTING "Enable testing" ON)
option(ENABLE_BENCHMARKS "Enable benchmarks" OFF)
option(ENABLE_SANITIZERS "Enable sanitizers" OFF)
option(ENABLE_STATIC_ANALYSIS "Enable static analysis" OFF)

# Find dependencies
include(FetchContent)

FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-28-0
)
FetchContent_MakeAvailable(asio)

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.7.0
)
FetchContent_MakeAvailable(benchmark)


find_package(OpenSSL)

# Add the main library target
add_library(mycppwebfw INTERFACE)

target_include_directories(mycppwebfw INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
  "${CMAKE_CURRENT_SOURCE_DIR}/devtools"
  "${asio_SOURCE_DIR}/asio/include"
  "${nlohmann_json_SOURCE_DIR}/include"
)

if(OpenSSL_FOUND)
  target_link_libraries(mycppwebfw INTERFACE OpenSSL::SSL)
endif()

# Compiler-specific optimizations
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(mycppwebfw INTERFACE -O3 -march=native)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(mycppwebfw INTERFACE -O3 -march=native -flto)
endif()

# Static analysis integration
if(ENABLE_STATIC_ANALYSIS)
  find_program(CLANG_TIDY clang-tidy)
  if(CLANG_TIDY)
      set_target_properties(mycppwebfw PROPERTIES
          CXX_CLANG_TIDY "${CLANG_TIDY};-checks=-*,readability-*,performance-*,modernize-*"
      )
  endif()
endif()

# Sanitizers for development
if(ENABLE_SANITIZERS)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(mycppwebfw INTERFACE -fsanitize=address,thread,undefined -fno-omit-frame-pointer)
    target_link_libraries(mycppwebfw INTERFACE -fsanitize=address,thread,undefined)
  endif()
endif()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(devtools)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

if(ENABLE_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()

add_subdirectory(examples)
