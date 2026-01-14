cmake_minimum_required(VERSION 3.10.0)
project(skynet-cpp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用C++23，如果编译器不支持会报错
set(CMAKE_CXX_EXTENSIONS OFF)        # 禁用编译器扩展（如GNU扩展），使用纯ISO C++

# 3rd
add_subdirectory(3rd/googletest)
# end 3rd

add_subdirectory(src)
add_subdirectory(test)
