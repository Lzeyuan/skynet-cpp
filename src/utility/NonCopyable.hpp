#pragma once
#ifndef UTILITY_NONCOPYABLE_HPP_
#define UTILITY_NONCOPYABLE_HPP_

namespace skynet::utility {
struct NonCopyable {
  NonCopyable() = default;
  ~NonCopyable() = default;

  NonCopyable(const NonCopyable &) = delete;
  NonCopyable &operator=(const NonCopyable &) = delete;
  NonCopyable(NonCopyable &&) = default;
  NonCopyable &operator=(NonCopyable &&) = default;
};

struct NonMovable {
  NonMovable() = default;
  ~NonMovable() = default;

  NonMovable(const NonMovable &) = delete;
  NonMovable &operator=(const NonMovable &) = delete;
  NonMovable(NonMovable &&) = delete;
  NonMovable &operator=(NonMovable &&) = delete;
};
} // namespace skynet::utility
#endif // UTILITY_NONCOPYABLE_HPP_
