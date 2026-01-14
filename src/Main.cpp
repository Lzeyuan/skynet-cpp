#include <print>

#include "Spinlock.hpp"

int main() {
  std::println("hello skynet-cpp.");

  skynet::sync::Spinlock spinlock;

  return 0;
}
