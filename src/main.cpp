#include <iostream> 
#include <boost/program_options.hpp>
#include <WorkerSystem.hpp>
#include <signal.h> //  our new library 
volatile sig_atomic_t flag = 0;
void signal_handler(int sig);

class TestWorker1 : public BaseWorker {
public:
  virtual void prepare() {
    std::cout << "Preparing: " << __PRETTY_FUNCTION__ << std::endl;
    workerName_ = __PRETTY_FUNCTION__;
  }
  TestWorker1() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual ~TestWorker1() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }

};

class TestWorker2 : public BaseWorker {
  public:
  virtual void prepare() {
    std::cout << "Preparing: " << __PRETTY_FUNCTION__ << std::endl;
    workerName_ = __PRETTY_FUNCTION__;
  }
  TestWorker2() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual ~TestWorker2() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }
};

class TestWorker3 : public BaseWorker {
  public:
  virtual void prepare() {
    std::cout << "Preparing: " << __PRETTY_FUNCTION__ << std::endl;
    workerName_ = __PRETTY_FUNCTION__;
  }

  TestWorker3() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
  virtual ~TestWorker3() {
    std::cout << "Destroying: " << __PRETTY_FUNCTION__ << std::endl;
  }
};

WorkerDispatcher<TestWorker1,TestWorker2,TestWorker3> dispatcher;

int main(int argc, char **argv) {
  std::cout << "Starting WorkerSystem" <<std::endl;
  signal(SIGINT, signal_handler); 

  dispatcher.start();
  dispatcher.join();
}

void signal_handler(int sig){ // can be called asynchronously
  std::cout << "shutting down" << std::endl;
  flag = 0; // set flag
  dispatcher.stop();
}