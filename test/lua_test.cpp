#include <gtest/gtest.h>

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

class LuaTest_NewState_Test : public ::testing::Test {
protected:
  lua_State *L;

  void SetUp() override {
    // 创建一个新的 Lua 虚拟机
    L = luaL_newstate();
    ASSERT_NE(L, nullptr);

    // 打开标准库
    luaL_openlibs(L);
  }

  void TearDown() override {
    if (L) {
      lua_close(L);
      L = nullptr;
    }
  }
};

// 测试能否执行简单的 Lua 脚本
TEST_F(LuaTest_NewState_Test, ExecuteSimpleScript) {
  const char *script = "return 2 + 3";

  int status = luaL_loadstring(L, script) || lua_pcall(L, 0, 1, 0);
  ASSERT_EQ(status, 0) << "Lua script failed to run";

  // 从栈顶取返回值
  ASSERT_TRUE(lua_isnumber(L, -1));
  double result = lua_tonumber(L, -1);

  EXPECT_EQ(result, 5);

  // 弹出栈顶元素
  lua_pop(L, 1);
}

// 测试能否设置全局变量并读取
TEST_F(LuaTest_NewState_Test, GlobalVariable) {
  lua_pushnumber(L, 42);
  lua_setglobal(L, "x");

  const char *script = "return x * 2";

  int status = luaL_loadstring(L, script) || lua_pcall(L, 0, 1, 0);
  ASSERT_EQ(status, 0);

  double result = lua_tonumber(L, -1);
  EXPECT_EQ(result, 84);

  lua_pop(L, 1);
}
