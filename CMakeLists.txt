cmake_minimum_required(VERSION 3.20)

project(hawkbit-cpp VERSION 0.1.0)

include(FetchContent)

FetchContent_Declare(
  json 
  URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.0.2
)
FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(fmt)

find_package(Poco REQUIRED Net)

option(BUILD_TESTS "Build tests")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-result")

# ======================================================
#
# LIBRARY hawkbit-cpp
#
# ======================================================

configure_file(version.h.in version.h)
add_library(hawkbit-cpp 
    ${PROJECT_SOURCE_DIR}/src/hawkbit/api/HawkbitApiDdiV1.cpp
    ${PROJECT_SOURCE_DIR}/src/hawkbit/api/HttpClient.cpp
    ${PROJECT_SOURCE_DIR}/src/hawkbit/dto/parsing_logic.cpp
    ${PROJECT_SOURCE_DIR}/src/hawkbit/dto/serialization_logic.cpp
)
target_include_directories(hawkbit-cpp
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_BINARY_DIR}
)
target_link_libraries(hawkbit-cpp
    nlohmann_json::nlohmann_json
    fmt::fmt
    Poco::Net
)

# ======================================================
#
# TESTS
#
# ======================================================

if(BUILD_TESTS)
enable_testing()
include(CTest)
target_compile_options(hawkbit-cpp PRIVATE --coverage)
target_link_options(hawkbit-cpp PRIVATE --coverage)
add_subdirectory(tests)
endif()

# ======================================================
#
# COVERAGE
#
# ======================================================

set(GCOVR_ARGS_APPEND
    --exclude ${PROJECT_SOURCE_DIR}/src/inicpp.h
    --exclude ${PROJECT_SOURCE_DIR}/src/components/base
)

add_custom_target(coverage_html)
add_custom_target(coverage_clean)

set(COVERAGE_DIR ${PROJECT_BINARY_DIR}/coverage)
set(GCOVR_ARGS --filter src --exclude src/main.cpp --object-directory ${PROJECT_BINARY_DIR} ${GCOVR_ARGS_APPEND})

add_custom_target(${PROJECT_NAME}_coverage_clean
  COMMAND rm ${COVERAGE_DIR}/* || true
  COMMAND gcovr --delete
)

add_custom_target(${PROJECT_NAME}_coverage_html
  COMMAND mkdir -p ${COVERAGE_DIR}
  COMMAND gcovr ${GCOVR_ARGS} --html-details -o ${COVERAGE_DIR}/report.html
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_dependencies(coverage_html ${PROJECT_NAME}_coverage_html)
add_dependencies(coverage_clean ${PROJECT_NAME}_coverage_clean)
