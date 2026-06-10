cmake_minimum_required(VERSION 3.21)
project(situacion_problema2 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
include(CTest)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.17.0
)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(httplib json googletest)

add_library(student_core
    src/GraphParser.cpp
    src/NetworkBuilder.cpp
    src/RoutingAndFlow.cpp
    src/GeoDistricts.cpp
)

target_include_directories(student_core PUBLIC include)
target_link_libraries(student_core PUBLIC httplib::httplib nlohmann_json::nlohmann_json)

add_executable(situacion_problema2 src/main.cpp)
target_link_libraries(situacion_problema2 PRIVATE student_core httplib::httplib nlohmann_json::nlohmann_json)

add_executable(unit_tests tests/unit/test_grade_calculator.cpp)
target_link_libraries(unit_tests PRIVATE student_core GTest::gtest_main)

add_executable(integration_tests tests/integration/test_integration.cpp)
target_link_libraries(integration_tests PRIVATE student_core GTest::gtest_main httplib::httplib nlohmann_json::nlohmann_json)

include(GoogleTest)
gtest_discover_tests(unit_tests TEST_PREFIX unit.)
gtest_discover_tests(integration_tests TEST_PREFIX integration.)
