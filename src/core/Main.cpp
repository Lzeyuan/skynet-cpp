#include "SpinLock.hpp" // 你的 Spinlock 头文件
#include <iostream>
#include <thread>
#include <vector>

using namespace skynet::sync;

int main() {
  Spinlock lock;
  int counter = 0;

  auto worker = [&]() {
    for (int i = 0; i < 10000; ++i) {
      Spinlock::Guard guard(lock); // RAII
      ++counter;
    }
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < 8; ++i) {
    threads.emplace_back(worker);
  }

  for (auto &t : threads) {
    t.join();
  }

  std::cout << "Counter = " << counter << std::endl;
  // 正确值应该是 8 * 10000 = 80000
}
