enable_testing()

add_executable(spinlock_test spinlock_test.cpp)

target_link_libraries(spinlock_test PRIVATE skynet-core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(spinlock_test)
