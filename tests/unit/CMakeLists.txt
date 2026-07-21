
add_executable(
  testhawkbit_cpp_unit
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${HAWKBIT_CPP_SOURCES}
)
target_link_libraries(
  testhawkbit_cpp_unit
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    ${HAWKBIT_CPP_LIBS}
)
target_include_directories(
  testhawkbit_cpp_unit 
    PRIVATE 
      ${PROJECT_SOURCE_DIR}/src
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${HAWKBIT_CPP_INCLUDES}
)

target_compile_options(testhawkbit_cpp_unit PRIVATE --coverage)
target_link_options(testhawkbit_cpp_unit PRIVATE --coverage)

gtest_discover_tests(testhawkbit_cpp_unit)
