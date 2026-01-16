enable_testing()

add_executable(spinlock_test spinlock_test.cpp)
add_executable(lua_test lua_test.cpp)

target_link_libraries(spinlock_test PRIVATE skynet-core GTest::gtest_main)
target_link_libraries(lua_test PRIVATE lua::lua skynet-core GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(spinlock_test)
gtest_discover_tests(lua_test)
