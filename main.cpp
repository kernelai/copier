#include <folly/experimental/io/IoUring.h>
#include <folly/experimental/io/IoUringBackend.h>
#include <folly/init/Init.h>
#include <folly/io/async/EventBase.h>
#include <folly/executors/ThreadedExecutor.h>

#include <iostream>

#include "fibers.h"

int main(int argc, char** argv) {
  folly::Init Init(&argc, &argv);
  LOG(INFO) << "glog hello, world!";
  fibers();
}