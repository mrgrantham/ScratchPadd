#pragma once


#include <WorkerSystem.hpp>


class TestWorker3 : public BaseWorker {
  public:
  virtual void prepare()  {
    std::cout << "Preparing: " << __PRETTY_FUNCTION__ << std::endl;
    workerName_ = __PRETTY_FUNCTION__;
  }

  TestWorker3(Dispatcher &dispatcher) : BaseWorker(dispatcher) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual ~TestWorker3() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }
};