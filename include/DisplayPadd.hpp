#pragma once
#include <Base.hpp>
#include <spdlog/spdlog.h>
#include <Timer.hpp>
#include <Message.hpp>
class DisplayPadd : public ScratchPadd::Base {
private:
  ScratchPadd::Timer performanceTimer_;
  std::unique_ptr<Graphics> graphics_;
public:
  virtual void prepare() override {
    spdlog::info("Preparing: {}", __CLASS_NAME__ );
    setRepeatInterval(100);
    performanceTimer_.setTimerName(paddName_);
    performanceTimer_.start();
  }
  DisplayPadd(ScratchPadd::System *system) : Base(system) {
    spdlog::info("Constructing: {}",__CLASS_NAME__ );
    paddName_ = __CLASS_NAME__;
    graphics_ = std::make_unique<GL4>();
    // We dont want the work loop to sleep
    // TODO make this sleep/wake from semaphore
    work_thread_sleep_interval_ = 0;
  }
  virtual void starting() override {
    spdlog::info("Starting Window Setup");
    graphics_->setupWindow();
  }
  virtual void finishing() override {
    spdlog::info("[{}] Avg Repeating Interval: {}",paddName_,performanceTimer_.getAverageIntervalString());
    performanceTimer_.markTimeAndPrint();
    graphics_->tearDown();
  }

  virtual ~DisplayPadd() {
    spdlog::info("Destroying: {}", __CLASS_NAME__);

  }

  virtual void repeat() override {
    spdlog::info("DisplayPadd drawing at {:.3f}s interval",performanceTimer_.markAndGetInterval());
    if (!graphics_->draw()) {
      spdlog::info("Stop drawing");
    }
  }

  virtual bool runOnMainThread() override {
    return true;
  }

  virtual void receive(ScratchPadd::Message message) override {
    ScratchPadd::MessageVariant &messageVariant = *message.get();
    std::visit(overload{
        [](ScratchPadd::Message_Type::Triangle& message)       { std::cout << "Triangle: " << message <<"\n"; },
        [](ScratchPadd::Message_Type::Point& message)   { std::cout << "Point: " << message << "\n"; },
        [](ScratchPadd::Message_Type::Text& message)       { std::cout << "Text: " << message << "\n"; }
    }, messageVariant);
  }

};