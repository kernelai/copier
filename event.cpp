#include <folly/io/async/EventBase.h>

void startEventBase() {
  folly::EventBase evb;
  evb.loop();
}