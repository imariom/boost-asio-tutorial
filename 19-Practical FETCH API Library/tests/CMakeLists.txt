include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/e54519b09463cec3aea77a1739e02c97ca766da5.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(restpp fetch_website.cc)
target_link_libraries(restpp GTest::gtest_main restpp)

include(GoogleTest)
gtest_discover_tests(restpp_tests)