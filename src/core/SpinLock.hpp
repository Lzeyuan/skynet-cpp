#pragma once
#ifndef CORE_SPINLOCK_HPP_

#ifdef __x86_64__
#include <immintrin.h> // for _mm_pause on x86/x64
#else
#include <thread>
#endif

#include <atomic>

namespace skynet::sync {

template <typename SpinlockImpl> class SpinlockWrapper {
public:
  SpinlockWrapper() noexcept = default;
  ~SpinlockWrapper() noexcept = default;

  SpinlockWrapper(const SpinlockWrapper &) = delete;
  SpinlockWrapper &operator=(const SpinlockWrapper &) = delete;
  SpinlockWrapper(SpinlockWrapper &&) = delete;
  SpinlockWrapper &operator=(SpinlockWrapper &&) = delete;

  void lock() noexcept { lock_.lock(); }

  bool try_lock() noexcept { return lock_.try_lock(); }

  void unlock() noexcept { lock_.unlock(); }

  class Guard {
    SpinlockWrapper &spin_;

  public:
    explicit Guard(SpinlockWrapper &spin) noexcept : spin_(spin) { spin_.lock(); }
    ~Guard() noexcept { spin_.unlock(); }

    Guard(const Guard &) = delete;
    Guard &operator=(const Guard &) = delete;
    Guard(Guard &&) = delete;
    Guard &operator=(Guard &&) = delete;
  };

private:
  SpinlockImpl lock_;
};

#ifndef USE_PTHREAD_LOCK

class SpinlockSTL {
public:
  SpinlockSTL() noexcept = default;
  ~SpinlockSTL() noexcept = default;

  SpinlockSTL(const SpinlockSTL &) = delete;
  SpinlockSTL &operator=(const SpinlockSTL &) = delete;
  SpinlockSTL(SpinlockSTL &&) = delete;
  SpinlockSTL &operator=(SpinlockSTL &&) = delete;

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

private:
  std::atomic<int> lock_{0};
};

using Spinlock = SpinlockWrapper<SpinlockSTL>;

#else
#include <pthread.h>

class SpinlockMutex {
public:
  SpinlockMutex() noexcept { pthread_mutex_init(&lock_, NULL); }
  ~SpinlockMutex() noexcept { pthread_mutex_destroy(&lock_); }

  SpinlockMutex(const SpinlockMutex &) = delete;
  SpinlockMutex &operator=(const SpinlockMutex &) = delete;
  SpinlockMutex(SpinlockMutex &&) = delete;
  SpinlockMutex &operator=(SpinlockMutex &&) = delete;

  void lock() noexcept { pthread_mutex_lock(&lock_); }

  bool try_lock() noexcept { return pthread_mutex_trylock(&lock_) == 0; }

  void unlock() noexcept { pthread_mutex_unlock(&lock_); }

private:
  pthread_mutex_t lock_;
};
using Spinlock = SpinlockWrapper<SpinlockMutex>;
#endif
} // namespace skynet::sync

#endif // CORE_SPINLOCK_HPP_