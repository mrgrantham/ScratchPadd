#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <spdlog/spdlog.h>
#include <EventTimer.hpp>
#include <boost/lockfree/queue.hpp>

#ifdef _WIN32
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

inline std::string className(const std::string& classMethod)
{
    size_t scopeResolutionOpIndex = classMethod.find("::");
    if (scopeResolutionOpIndex == std::string::npos) {
        return "::";
    }
    size_t classNameStartIndex = classMethod.substr(0,scopeResolutionOpIndex).rfind(" ") + 1;
    size_t classNameLength = scopeResolutionOpIndex - classNameStartIndex;

    return classMethod.substr(classNameStartIndex,classNameLength);
}

#define __CLASS_NAME__ className(__PRETTY_FUNCTION__)

namespace ScratchPadd {

class Base {
  protected:
  bool on_{true};
  std::string paddName_;
  std::thread workerThread_;
  int repeating_interval_{0};
  int work_thread_sleep_interval_{1000};
  EventTimer repeatingTimer_;
  boost::lockfree::queue<std::function<void()>*> work_queue_{100};

  public:
  Base() {
    paddName_ = __CLASS_NAME__;
  }
  virtual ~Base() {
    // spdlog gets torn down before the destructor
    // need cout in order to know about destructors called
    // spdlog::info("Destroying: {}", __CLASS_NAME__);
    repeatingTimer_.stop();
    std::cout <<  "Base() Destroying: " << paddName_ << std::endl;
  }
  void start() {
    spdlog::info("Start: {}", paddName_ );
    if(!runOnMainThread()) {
      workerThread_ = std::thread(&Base::run,this);
    }
    startRepeater();
  }
  virtual bool runOnMainThread() {return false;}
  virtual void config(){}
  virtual void prepare(){}
  virtual void cleanup(){}
  virtual void starting(){}
  virtual void finishing(){}
  virtual void repeat(){
    spdlog::info("Base::repeat() called. Padd set a repeat interval of {} with no method override",repeating_interval_);
  }

  void runIfMainThread() {
    if (runOnMainThread()) {
      run_once();
    }
  }

  void startingIfMainThread() {
    if (runOnMainThread()) {
      starting();
    }
  }

  void finishingIfMainThread() {
    if (runOnMainThread()) {
      finishing();
    }
  }

  void run() {
    starting();
    while (on_) {
      loop();
    }
    finishing();
  }

  void run_once() {
    if (on_) {
      loop();
    }
  }

  void loop() {
      std::function<void()> *work = nullptr;
      while(work_queue_.pop(work)) {
        if (!work) spdlog::error("work is null");
        work->operator()();
        delete work;
      }
      // spdlog::warn("sleeping work_queue_ {} for {}ms",paddName_, work_thread_sleep_interval_);
      std::this_thread::sleep_for(std::chrono::milliseconds(work_thread_sleep_interval_));
  }

  void setRepeatInterval( int interval) {
    repeating_interval_ = interval;
  }

  void startRepeater() {
    if (repeating_interval_) {
      spdlog::info("repeat() interval set to {}",repeating_interval_);
      repeatingTimer_.startRepeatingEvent([=]{
        std::function<void()> work = std::function<void()>([=]{
          this->repeat();
        });
        send(work);
      },repeating_interval_);
    } else {
      spdlog::info("No repeat() interval set");
    }
  }

  template <typename Sendable>
  void send(Sendable sendable) {
    std::function<void()> *function = new std::function<void()>([=]{
      recieve(sendable);
    });
    work_queue_.push(function);
  }

  void recieve(const std::function<void()> &work) {
    work();
  }


  void recieve(void) {
    spdlog::error("Void Receive Called");
  }

  void stop() {
    spdlog::info("Stopping: {}", paddName_ );
    on_ = false;
    if (workerThread_.joinable()) {
      workerThread_.join();
    }
  }
};

}