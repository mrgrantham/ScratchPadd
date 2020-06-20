#pragma once

#include <chrono>
#include <thread>
#include <memory>
#include <spdlog/spdlog.h>

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

class BaseWorker {
  protected:
  bool on_{true};
  std::string workerName_;
  std::thread workerThread_;
  public:
  BaseWorker() {
    workerName_ = __CLASS_NAME__;
  }
  virtual ~BaseWorker() {
    // spdlog gets torn down before the destructor
    // need cout in order to know about destructors called
    // spdlog::info("Destroying: {}", __CLASS_NAME__);
    std::cout <<  "Destroying: " << __CLASS_NAME__ << std::endl;
  }
  void start() {
    spdlog::info("Starting: {}", workerName_ );
    workerThread_ = std::thread(&BaseWorker::run,this);
  }
  virtual void config(){}
  virtual void prepare(){}
  virtual void cleanup(){}
  virtual void starting(){}
  virtual void finishing(){}

  void run() {
    starting();
    while (on_) {
      spdlog::info("Working: {}", workerName_);
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    finishing();
  }

  void stop() {
    spdlog::info("Stopping: {}", workerName_ );
    on_ = false;
    workerThread_.join();
  }
};