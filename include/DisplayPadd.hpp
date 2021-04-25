#pragma once
#include <Base.hpp>
#include <spdlog/spdlog.h>
#include <Timer.hpp>
template <typename Graphics>
class DisplayPadd : public ScratchPadd::Base {
private:
  ScratchPadd::Timer performanceTimer_;
  Graphics graphics_;
public:
  virtual void prepare() override {
    spdlog::info("Preparing: {}", __CLASS_NAME__ );
    setRepeatInterval(100);
    performanceTimer_.setTimerName(paddName_);
    performanceTimer_.start();
  }
  DisplayPadd() : Base() {
    spdlog::info("Constructing: {}",__CLASS_NAME__ );
    paddName_ = __CLASS_NAME__;

    // We dont want the work loop to sleep
    // TODO make this sleep/wake from semaphore
    work_thread_sleep_interval_ = 0;
  }
  virtual void starting() override {
    spdlog::info("Starting Window Setup");
    graphics_.setupWindow();
  }
  virtual void finishing() override {
    spdlog::info("[{}] Avg Repeating Interval: {}",paddName_,performanceTimer_.getAverageIntervalString());
    performanceTimer_.markTimeAndPrint();
    graphics_.tearDown();
  }

  virtual ~DisplayPadd() {
    spdlog::info("Destroying: {}", __CLASS_NAME__);

  }

  virtual void repeat() override {
    spdlog::info("DisplayPadd drawing at {:.3f}s interval",performanceTimer_.markAndGetInterval());
    if (!graphics_.draw()) {
      spdlog::info("Stop drawing");
    }
  }

  virtual bool runOnMainThread() override {
    return true;
  }

};