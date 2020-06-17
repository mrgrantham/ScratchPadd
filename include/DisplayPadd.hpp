#pragma once


#include <WorkerSystem.hpp>

class DisplayPadd : public BaseWorker {
public:
  virtual void prepare() {
    std::cout << "Preparing: " << __PRETTY_FUNCTION__ << std::endl;
    workerName_ = __PRETTY_FUNCTION__;
  }
  DisplayPadd(Dispatcher &dispatcher) : BaseWorker(dispatcher) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual ~DisplayPadd() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }

};