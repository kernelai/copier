#include <iostream>
#include <folly/experimental/io/IoUringBackend.h>
#include "fibers.h"

int main() { std::cout << "Hello, World!" << std::endl;
  fibers();

 }