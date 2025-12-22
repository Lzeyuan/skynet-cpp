#pragma once

#ifndef SPINLOCK_HPP_
#define SPINLOCK_HPP_

#include <atomic>
#if defined(__x86_64__)
#include <immintrin.h>
#define cpu_pause() _mm_pause()
#else
#define cpu_pause() ((void)0)
#endif

namespace core {
class SpinLock {
public:
  SpinLock() noexcept { lock_.store(0, std::memory_order_release); }

  void lock() noexcept {
    for (;;) {
      // 第一层：尝试 test-and-set
      if (!lock_.exchange(1, std::memory_order_acquire))
        return;

      // 第二层：只读等待，避免 cache bouncing
      while (lock_.load(std::memory_order_relaxed)) {
        cpu_pause();
      }
    }
  }

  bool try_lock() noexcept {
    return !lock_.load(std::memory_order_relaxed) &&
           !lock_.exchange(1, std::memory_order_acquire);
  }

  void unlock() noexcept { lock_.store(0, std::memory_order_release); }

private:
  std::atomic<int> lock_{0};
};
} // namespace core
#endif // SPINLOCK_HPP_