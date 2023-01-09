#include <iostream>
#include <folly/io/async/EventBase.h>
#include <folly/fibers/Baton.h>
#include <folly/fibers/FiberManager.h>
#include <folly/fibers/FiberManagerMap.h>
#include <folly/fibers/Fiber.h>



int main() {
  std::cout << "Hello, World!" << std::endl;
  folly::EventBase evb;
  auto& fiberManager = folly::fibers::getFiberManager(evb);
  folly::fibers::Baton baton;
  fiberManager.addTask([&]() {
    std::cout << "Task 1: start" << std::endl;
    baton.wait();
    std::cout << "Task 1: after baton.wait()" << std::endl;
  });

  fiberManager.addTask([&]() {
    std::cout << "Task 2: start" << std::endl;
    baton.post();
    std::cout << "Task 2: after baton.post()" << std::endl;
  });

  evb.loop();
}