
#pragma once
#include <spdlog/spdlog.h>
#include <atomic>
#include <thread>

namespace shutdown {
inline void run(std::atomic<bool>& stop, bool stress) {
  auto logger = spdlog::default_logger();
  std::thread t([&] {
    while (!stop.load()) {
      if (logger) logger->info("shutdown now");
    }
  });

  if (stress) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  stop.store(true);
  t.join();
}
}
