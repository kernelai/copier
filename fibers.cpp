#include "fibers.h"

#include <folly/executors/FiberIOExecutor.h>
#include <folly/executors/IOThreadPoolExecutor.h>
#include <folly/fibers/Baton.h>
#include <folly/fibers/Fiber.h>
#include <folly/fibers/FiberManager.h>
#include <folly/fibers/FiberManagerMap.h>
#include <folly/fibers/Promise.h>
#include <folly/io/async/EventBase.h>

#include <iostream>

void fibers() {
  auto executer =
      folly::FiberIOExecutor(std::make_shared<folly::IOThreadPoolExecutor>(
          1, std::make_shared<folly::NamedThreadFactory>("fiber Thread")));
  executer.add([]() { std::cout << "fiber, Hello, World!" << std::endl; });
}

void history() {
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