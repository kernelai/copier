#include <glog/logging.h>
#include <folly/init/Init.h>
#include <folly/logging/Init.h>
#include <folly/logging/xlog.h>

#include <iostream>

#include "fibers.h"
using namespace std;

// FOLLY_INIT_LOGGING_CONFIG(
    // ".=INFO,folly=INFO; default:async=true,sync_level=INFO");

int main(int argc, char** argv) {
  folly::Init Init(&argc, &argv);
  RunFiber();
}