#pragma once

#include <WorkerDispatcher.hpp>
#include <chrono>
#include <thread>
#include <memory>


class BaseWorker {
  protected:
  bool on_{true};
  std::string workerName_;
  std::thread workerThread_;
  Dispatcher &dispatcher_;
  public:
  BaseWorker(Dispatcher &dispatcher): dispatcher_(dispatcher) {
    workerName_ = __PRETTY_FUNCTION__;
  }
  virtual ~BaseWorker() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }
  void start() {
    std::cout << "Starting: " << workerName_ << std::endl;
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
      std::cout << "working: " << workerName_ << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    finishing();
  }

  void stop() {
    on_ = false;
    workerThread_.join();
  }

  template <typename Data, typename ...Workers>
  void sendData(Data &data) {
    dispatcher_.sendData<Data,Workers...>(data);
  }

  template <typename Work, typename ...Workers>
  void sendWork(Work &work) {
    // dispatcher_<Workers>->sendWork()
  }
};