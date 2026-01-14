add_library(skynet-core
    STATIC
    LuaWrapper.cpp
)

target_include_directories(skynet-core PUBLIC
    .
)

target_link_libraries(skynet-core PUBLIC
    skynet-utility
)
