#pragma once
#ifndef CORE_SPINLOCK_HPP_

#ifdef __x86_64__
#include <immintrin.h> // for _mm_pause on x86/x64
#else
#include <thread>
#endif

#include <atomic>

#include "NonCopyable.hpp"

namespace skynet::sync {

class SpinlockSTL : utility::NonMovable {
public:
  SpinlockSTL() noexcept = default;
  ~SpinlockSTL() noexcept = default;

  void lock() noexcept {
    for (;;) {
      int expected = 0;
      if (lock_.compare_exchange_strong(expected, 1,
                                        std::memory_order_acquire)) {
        return;
      }
      while (lock_.load(std::memory_order_relaxed)) {
#ifdef __x86_64__
        _mm_pause();
#else
        std::this_thread::yield()
#endif
      }
    }
  }

  bool try_lock() noexcept {
    int expected = 0;
    return lock_.compare_exchange_strong(expected, 1,
                                         std::memory_order_acquire);
  }

  void unlock() noexcept { lock_.store(0, std::memory_order_release); }

  class Guard : utility::NonMovable {
    SpinlockSTL &spin_;

  public:
    explicit Guard(SpinlockSTL &spin) noexcept : spin_(spin) { spin_.lock(); }
    ~Guard() noexcept { spin_.unlock(); }

    Guard(const Guard &) = delete;
    Guard &operator=(const Guard &) = delete;
    Guard(Guard &&) = delete;
    Guard &operator=(Guard &&) = delete;
  };

private:
  std::atomic<int> lock_{0};
};

using Spinlock = SpinlockSTL;

} // namespace skynet::sync

#endif // CORE_SPINLOCK_HPP_
