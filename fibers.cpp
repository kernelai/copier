#include "fibers.h"

#include <folly/executors/FiberIOExecutor.h>
#include <folly/executors/IOThreadPoolExecutor.h>
#include <folly/fibers/Baton.h>
#include <folly/fibers/Fiber.h>
#include <folly/fibers/FiberManager.h>
#include <folly/fibers/FiberManagerMap.h>
#include <folly/futures/Promise.h>
// #include <folly/futures/Future.h>
#include <folly/io/async/EventBase.h>

#include <iostream>

void RunFibers() {
  auto executer =
      folly::FiberIOExecutor(std::make_shared<folly::IOThreadPoolExecutor>(
          1, std::make_shared<folly::NamedThreadFactory>("fiber Thread")));
  executer.add([]() { std::cout << "fiber, Hello, World!" << std::endl; });
  folly::Promise<int> p;
  auto f = p.getSemiFuture();
  executer.add([&]() mutable {
    std::cout << "f1 waite for future, state: " << (f.valid() == true
        ? "ture "
        : "false") << std::endl;

    auto const v = std::move(f).get();
    std::cout << "f1 get feature value: " << v << std::endl;
  });
  executer.add([&]() {
    std::cout << "f2 set feature value" << std::endl;
    p.setValue(42);
  });
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