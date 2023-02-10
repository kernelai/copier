#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>
#include <folly/init/Init.h>

#include <iostream>

#include "fibers.h"
using namespace std;

int main(int argc, char** argv) {
  folly::Init Init(&argc, &argv);
  RunFiber();
}