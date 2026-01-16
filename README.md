# skynet-cpp

#

# 约定
## C++版本26
- println

## spinlock
只用标准库实现

## NonCopyable.hpp
| 类型 | 复制 | 移动 |
|------|------|------|
| NonCopyable | ❌ 不允许 | ✅ 允许 |
| NonMovable | ❌ 不允许 | ❌ 不允许 |

## daemon 守护进程
使用systemd
