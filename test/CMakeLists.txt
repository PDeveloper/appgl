add_subdirectory(googletest)

# Define your test executable
add_executable(tests
    tests.cpp
)

# Link your test executable with Google Test and your application
target_link_libraries(tests
    gtest_main
    # Add any other libraries your tests depend on
)

# Register your tests
include(GoogleTest)
gtest_discover_tests(tests)
