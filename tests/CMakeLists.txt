set(GOOGLETEST_REV 03597a01ee50ed33e9dfd640b249b4be3799d395)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/${GOOGLETEST_REV}.zip
)
set(gtest_build_tests OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_subdirectory(unit)