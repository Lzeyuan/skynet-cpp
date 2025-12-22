#pragma once
#ifndef SKYNET_H_

#include "Spinlock.hpp"

namespace core {
class SkynetEnv {
private:
    SpinLock spinLock_;
    lua_State *L;
};
} // namespace core

#endif // SKYNET_H_