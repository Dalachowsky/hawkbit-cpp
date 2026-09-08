
# Required by testcontainers-cpp
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
  test_hawkbit_cpp_integration_hawkbit_api
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${HAWKBIT_CPP_SOURCES}
)
target_link_libraries(
  test_hawkbit_cpp_integration_hawkbit_api
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    hawkbit-cpp
    ${HAWKBIT_CPP_LIBS}
)
target_include_directories(
  test_hawkbit_cpp_integration_hawkbit_api
    PRIVATE 
      ${PROJECT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${HAWKBIT_CPP_INCLUDES}
)

target_compile_options(test_hawkbit_cpp_integration_hawkbit_api PRIVATE --coverage)
target_link_options(test_hawkbit_cpp_integration_hawkbit_api PRIVATE --coverage)

gtest_discover_tests(test_hawkbit_cpp_integration_hawkbit_api)
