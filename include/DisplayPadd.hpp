#pragma once
#include <ScratchPadd/Base.hpp>
#include <spdlog/spdlog.h>
#include <ScratchPadd/Timer.hpp>
#include <Message.hpp>
#include <ScratchPadd/Graphics/Graphics.hpp>
class DisplayPadd : public ScratchPadd::Base {
private:
  ScratchPadd::Timer performanceTimer_;
  std::unique_ptr<Graphics> graphics_;
  std::unique_ptr<Graphics::View> view_;
public:
  virtual void prepare() override {
    spdlog::info("Preparing: {}", __CLASS_NAME__ );
    setRepeatInterval(32);
    performanceTimer_.setTimerName(paddName_);
    performanceTimer_.start();
  }
  DisplayPadd(ScratchPadd::System *system) : Base(system) {
    spdlog::info("Constructing: {}",__CLASS_NAME__ );
    paddName_ = __CLASS_NAME__;
    graphics_ = GraphicsBuilder();
    view_ = ViewBuilder();
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
    performanceTimer_.markInterval();
    static int count = 0;
    if (count % 30 == 0) {
      spdlog::info("DisplayPadd drawing at {:.3f}s interval",performanceTimer_.getAverageIntervalInSeconds(5));
      spdlog::info("DisplayPadd drawing at {:.3f} fps",1.0/performanceTimer_.getAverageIntervalInSeconds(5));
    }
    count++;

    if (!graphics_->draw()) {
      spdlog::info("Stop drawing");
      system_->stop();
    }
  }

  virtual bool runOnMainThread() override {
    return true;
  }

  virtual void receive(ScratchPadd::Message message) override {
    ScratchPadd::MessageVariant &messageVariant = *message.get();
    std::visit(overload{
        [&](ScratchPadd::Message_Type::Triangle& message)       { std::cout << paddName_ << "Triangle: " << message <<"\n"; },
        [&](ScratchPadd::Message_Type::Point& message)   { std::cout << paddName_ <<"Point: " << message << "\n"; },
        [&](ScratchPadd::Message_Type::Text& message)       { std::cout << paddName_ << "Text: " << message << "\n"; },
        [&](ScratchPadd::Message_Type::Control& message)       { std::cout << paddName_ << "Control " << "\n"; }
    }, messageVariant);
  }

};