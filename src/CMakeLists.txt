add_subdirectory(core)
add_subdirectory(utility)

add_executable(skynet-cpp
    Main.cpp
)

target_link_libraries(skynet-cpp PRIVATE
    skynet-core
)
