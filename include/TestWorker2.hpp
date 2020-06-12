#pragma once


#include <WorkerSystem.hpp>

class TestWorker2 : public BaseWorker {
  public:
  virtual void prepare() {
    std::cout << "Preparing: " << typeid(*this).name() << std::endl;
    workerName_ = typeid(*this).name();
  }
  TestWorker2(Dispatcher &dispatcher)  : BaseWorker(dispatcher){
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual ~TestWorker2() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }
};