#ifndef TEST_WORKER_1_HPP
#define TEST_WORKER_1_HPP

#include <WorkerSystem.hpp>

class TestWorker1 : public BaseWorker {
public:
  virtual void prepare() {
    std::cout << "Preparing: " << __PRETTY_FUNCTION__ << std::endl;
    workerName_ = __PRETTY_FUNCTION__;
  }
  TestWorker1(Dispatcher &dispatcher) : BaseWorker(dispatcher) {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual ~TestWorker1() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }

};

#endif