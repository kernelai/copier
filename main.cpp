#include <iostream>
#include <glog/logging.h>
#include <folly/init/Init.h>

#include "fibers.h"
using namespace std;

int main(int argc, char** argv) {
  folly::Init Init(&argc, &argv);
  LOG(INFO) << "Hello World!";
  RunFiber();
}