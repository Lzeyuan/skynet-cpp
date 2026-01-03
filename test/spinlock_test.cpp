#include "SpinLock.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

using namespace skynet::sync;

TEST(SpinlockTest, CounterIncrement) {
  Spinlock lock;
  int counter = 0;

  auto worker = [&]() {
    for (int i = 0; i < 10000; ++i) {
      Spinlock::Guard g(lock);
      ++counter;
    }
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < 8; ++i)
    threads.emplace_back(worker);
  for (auto &t : threads)
    t.join();

  EXPECT_EQ(counter, 80000);
}

TEST(SpinlockTest, TryLockWorks) {
  Spinlock lock;
  EXPECT_TRUE(lock.try_lock());
  lock.unlock();
}

TEST(SpinlockTest, GuardExceptionSafety) {
  Spinlock lock;
  int counter = 0;

  auto worker = [&]() {
    for (int i = 0; i < 1000; ++i) {
      try {
        Spinlock::Guard g(lock);
        ++counter;
        if (i % 100 == 0) {
          throw std::runtime_error("test exception");
        }
      } catch (const std::runtime_error&) {
        // 捕获异常，继续循环
      }
    }
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < 4; ++i) threads.emplace_back(worker);
  for (auto& t : threads) t.join();

  // 检查锁是否仍可用，保证 counter 正确累加
  {
    Spinlock::Guard g(lock);
    counter += 1;  // 如果锁未释放，这里会死锁
  }

  EXPECT_GT(counter, 0);
}
