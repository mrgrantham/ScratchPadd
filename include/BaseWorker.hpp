#ifndef BASEWORKER_HPP
#define BASEWORKER_HPP

#include <chrono>
#include <thread>

class BaseWorker {
  protected:
  bool on_{true};
  std::string workerName_;
  std::thread workerThread_;
  public:
  BaseWorker() {
    workerName_ = __PRETTY_FUNCTION__;
  }
  virtual ~BaseWorker() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }
  void start() {
    std::cout << "Starting: " << workerName_ << std::endl;
    workerThread_ = std::thread(&BaseWorker::run,this);
  }
  virtual void prepare(){}
  virtual void cleanup(){}
  virtual void starting(){}
  virtual void finishing(){}
  void run() {
    starting();
    while (on_) {
      std::cout << "working: " << workerName_ << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    finishing();
  }
  void stop() {
    on_ = false;
  }
};

typedef void (BaseWorker::*WorkerMethod)();

#endif