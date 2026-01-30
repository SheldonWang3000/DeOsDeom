
#pragma once
#include <spdlog/spdlog.h>
#include <atomic>
#include <thread>
#include <string>

namespace level {
inline int g_level = 0;

class Level5
{
public:
  Level5(std::string ss): s(std::move(ss)) {};
  ~Level5() {};

  void print() {
    spdlog::debug("{}", s);
  }

private:
  std::string s;
};

inline void run(std::atomic<bool>& stop, bool) {
  std::thread r([&] {
    while (!stop.load()) {
      g_level = (g_level + 1) % 1000000006;
      if (g_level & 1) spdlog::info("odd");
      else spdlog::info("even");
    }
  });
  r.join();
}
}
