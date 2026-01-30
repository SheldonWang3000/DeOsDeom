
#pragma once
#include <spdlog/sinks/base_sink.h>
#include <spdlog/spdlog.h>
#include <atomic>
#include <thread>
#include <queue>
#include <string>
#include <condition_variable>

namespace sink_callback {

class ReentrantSink : public spdlog::sinks::base_sink<std::mutex> {
public:
  bool print(std::string& msg, std::atomic<bool>& stop) {
    std::unique_lock<std::mutex> lk(queueMutex);
    cv.wait(lk, [&] { return stop.load() || !this->msgQueue.empty(); }); 
    if (stop.load()) return false;
    msg = std::move(this->msgQueue.front());
    this->msgQueue.pop();
    return true;
  }
  void notify_all() {
    this->cv.notify_all();
  }
protected:
  void sink_it_(const spdlog::details::log_msg&) override {
    std::lock_guard<std::mutex> lk(queueMutex);
    this->msgQueue.push("re-enter logger");
    this->cv.notify_one();
  }
  void flush_() override {}
private:
  std::queue<std::string> msgQueue;
  std::mutex queueMutex;
  std::condition_variable cv;
};

inline void run(std::atomic<bool>& stop, bool stress) {
  if (!stress) return;
  auto sink = std::make_shared<ReentrantSink>();
  std::thread t([&] {
    std::string msg;
    while (!stop.load()) {
      if (!sink->print(msg, stop)) break;
      spdlog::info("{}", msg);
    }
  });
  auto logger = std::make_shared<spdlog::logger>("reentrant", sink);
  while (!stop.load()) {
    logger->warn("trigger");
  }
  sink->notify_all();
  t.join();
}
}
