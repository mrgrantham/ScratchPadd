#include <iostream> 
#include <boost/program_options.hpp>
#include <WorkerSystem.hpp>

class TestWorker1 : public BaseWorker {
public:
  TestWorker1() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }

};

class TestWorker2 : public BaseWorker {
  public:
  TestWorker2() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

class TestWorker3 : public BaseWorker {
  public:
  TestWorker3() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
  }
};

int main(int argc, char **argv) {
  std::cout << "Starting WorkerSystem" <<std::endl;

  WorkerDispatcher dispatcher;
  dispatcher.initialize<TestWorker1,TestWorker2,TestWorker3> ();
  
}