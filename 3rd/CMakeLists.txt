add_subdirectory(googletest)

# lua
include(ExternalProject)

set(LUA_SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rd/lua)
set(LUA_LIB_PATH   ${LUA_SOURCE_DIR}/src/liblua.a)

ExternalProject_Add(lua_external
    SOURCE_DIR ${LUA_SOURCE_DIR}

    BINARY_DIR  ${LUA_SOURCE_DIR}

    CONFIGURE_COMMAND ""

    BUILD_COMMAND
        make linux

    INSTALL_COMMAND ""

    BUILD_BYPRODUCTS
        ${LUA_LIB_PATH}
)

add_library(lua::lua STATIC IMPORTED GLOBAL)

set_target_properties(lua::lua PROPERTIES
    IMPORTED_LOCATION
        ${LUA_LIB_PATH}

    INTERFACE_INCLUDE_DIRECTORIES
        ${LUA_SOURCE_DIR}/src
)

add_dependencies(lua::lua lua_external)

# end lua
