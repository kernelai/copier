#include "fibers.h"

#include <folly/FileUtil.h>
#include <folly/String.h>
#include <folly/executors/FiberIOExecutor.h>
#include <folly/executors/IOThreadPoolExecutor.h>
#include <folly/experimental/TestUtil.h>
#include <folly/experimental/io/IoUring.h>
#include <folly/experimental/io/IoUringBackend.h>
#include <folly/fibers/Baton.h>
#include <folly/fibers/Fiber.h>
#include <folly/fibers/FiberManager.h>
#include <folly/fibers/FiberManagerMap.h>
#include <folly/futures/Promise.h>
#include <folly/io/async/EventBase.h>
#include <folly/logging/xlog.h>

#include <iostream>
#include <random>

constexpr size_t kODirectAlign = 4096;  // align reads to 4096 B (for O_DIRECT)

using ManagedBuffer = std::unique_ptr<char, void (*)(void*)>;

ManagedBuffer allocateAligned(size_t size) {
  void* buf;
  int rc = posix_memalign(&buf, kODirectAlign, size);
  CHECK_EQ(rc, 0) << folly::errnoStr(rc);
  return ManagedBuffer(reinterpret_cast<char*>(buf), free);
}

class TempFileUtil {
 public:
  // Returns a temporary file that is NOT kept open
  // but is deleted on destruction
  // Generate random-looking but reproduceable data.
  static folly::test::TemporaryFile getTempFile(size_t size) {
    CHECK_EQ(size % sizeof(uint32_t), 0);
    size /= sizeof(uint32_t);

    folly::test::TemporaryFile tmpFile;
    int fd = tmpFile.fd();
    CHECK_GE(fd, 0);

    // fill the file the file with random data
    const uint32_t seed = 42;
    std::mt19937 rnd(seed);

    constexpr size_t bufferSize = 1U << 16;
    uint32_t buffer[bufferSize];

    while (size) {
      size_t n = std::min(size, bufferSize);
      for (size_t i = 0; i < n; ++i) {
        // buffer[i] = rnd();
        buffer[i] = i;
      }
      size_t written = folly::writeFull(fd, buffer, sizeof(uint32_t) * n);
      CHECK_EQ(written, sizeof(uint32_t) * n);
      size -= n;
    }

    CHECK_EQ(::fdatasync(fd), 0);

    // the file was opened with O_EXCL so we need to close to be able
    // to open it again
    tmpFile.close();

    return tmpFile;
  }

 private:
  TempFileUtil() = delete;
};

folly::EventBase getEventBase(folly::PollIoBackend::Options opts) {
  try {
    auto factory = [opts] {
      return std::make_unique<folly::IoUringBackend>(opts);
    };
    return folly::EventBase(
        folly::EventBase::Options().setBackendFactory(std::move(factory)));
  } catch (const folly::IoUringBackend::NotAvailable&) {
    return folly::EventBase();
  }
}

folly::EventBase getEventBase() {
  static constexpr size_t kBackendCapacity = 32;
  static constexpr size_t kBackendMaxSubmit = 16;
  static constexpr size_t kBackendMaxGet = 8;
  folly::PollIoBackend::Options options;
  options.setCapacity(kBackendCapacity)
      .setMaxSubmit(kBackendMaxSubmit)
      .setMaxGet(kBackendMaxGet)
      .setUseRegisteredFds(0);
  return getEventBase(options);
}

void RunFiber() {
  auto tempFile = TempFileUtil::getTempFile(1024 * 1024);

  auto evb = getEventBase();
  auto& fiberManager = folly::fibers::getFiberManager(evb);
  fiberManager.addTask([&]() {
    constexpr size_t kNumEntries = 2;
    constexpr size_t kBufSize = 4096;
    auto backend = dynamic_cast<folly::IoUringBackend*>(evb.getBackend());

    folly::Promise<int> p;
    auto f1 = p.getFuture();
    backend->queueOpenat(
        0, tempFile.path().c_str(), O_DIRECT | O_RDWR, 0, [&](int res) {
          if (res < 0) {
            p.setException(std::runtime_error("IO Uring openat error:" +
                                              folly::errnoStr(-res)));
            return;
          }
          p.setValue(res);
        });
    auto fd = std::move(f1).get();
    folly::Promise<int> promise;
    auto f = promise.getFuture();
    auto buf = ManagedBuffer(allocateAligned(kBufSize));
    backend->queueRead(fd, buf.get(), kBufSize, 0, [&](int res) {
      if (res < 0) {
        promise.setException(std::runtime_error("IO Uring read error"));
        return;
      }
      promise.setValue(res);
    });

    try {
      auto res = std::move(f).get();
      XLOG(INFO) << "read res: " << res << std::endl;
    } catch (const std::exception& e) {
      XLOG(INFO) << "read error: " << e.what() << std::endl;
    }
  });

  evb.loopForever();
}