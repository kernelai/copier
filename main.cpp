#include <folly/executors/ThreadedExecutor.h>
#include <folly/futures/Future.h>
#include <folly/init/Init.h>

#include <iostream>

#include "fibers.h"
// using namespace folly;
using namespace std;

void foo(int x) {
  // do something with x
  cout << "foo(" << x << ")" << endl;
}
void test() {
  folly::ThreadedExecutor executor;
  cout << "making Promise" << endl;
  folly::Promise<int> p;
  folly::Future<int> f = p.getFuture().via(&executor);

  // ... now perhaps in another event callback

  cout << "fulfilling Promise" << endl;
  p.setValue(42);
  move(f).get();
  cout << "Promise fulfilled" << endl;
}
int main(int argc, char** argv) {
  folly::Init Init(&argc, &argv);
  // RunFibers();
  RunfiberInManager();
}