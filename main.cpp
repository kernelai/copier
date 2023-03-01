#include <iostream>
#include <glog/logging.h>

// #include "fibers.h"
using namespace std;

int main(int argc, char** argv) {
  // folly::Init Init(&argc, &argv);
  // RunFiber();
  std::cout << "Hello World!" << std::endl;
  LOG(INFO) << "Hello World!";
}