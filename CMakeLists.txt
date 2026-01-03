cmake_minimum_required(VERSION 3.10.0)
project(skynet-cpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用C++23，如果编译器不支持会报错
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（如GNU扩展），使用纯ISO C++

add_subdirectory(3rd/googletest)

add_executable(skynet-cpp 
    src/core/Main.cpp
)

enable_testing()

add_executable(spinlock_test test/spinlock_test.cpp)

target_include_directories(spinlock_test PRIVATE src/core)
target_link_libraries(spinlock_test PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(spinlock_test)