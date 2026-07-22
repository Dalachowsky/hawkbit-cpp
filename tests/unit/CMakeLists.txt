
add_executable(
  test_hawkbit_cpp_unit
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
target_link_libraries(
  test_hawkbit_cpp_unit
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    nlohmann_json::nlohmann_json
    fmt
    hawkbit-cpp
)
target_include_directories(
  test_hawkbit_cpp_unit 
    PRIVATE 
      ${PROJECT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_options(test_hawkbit_cpp_unit PRIVATE --coverage)
target_link_options(test_hawkbit_cpp_unit PRIVATE --coverage)

gtest_discover_tests(test_hawkbit_cpp_unit)
