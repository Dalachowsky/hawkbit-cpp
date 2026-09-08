include(GoogleTest)

set(GOOGLETEST_REV 03597a01ee50ed33e9dfd640b249b4be3799d395)
set(gtest_build_tests OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/${GOOGLETEST_REV}.zip
)
FetchContent_MakeAvailable(googletest)
#FetchContent_Declare(
#    testcontainers
#    GIT_REPOSITORY https://github.com/testcontainers/testcontainers-native
#    GIT_TAG 2861a964ab292ec04434646de374d5e4a1be7ccd
#)
#FetchContent_MakeAvailable(testcontainers)

add_subdirectory(integration)
add_subdirectory(unit)