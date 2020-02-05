#include <BaseWorker.hpp>

class WorkerDispatcher {
  std::vector<BaseWorker> modules_;
  public:
  WorkerDispatcher() {

  }

  template <typename ...Workers>
  void initialize() {
    std::cout << "initializing WorkerDispatcher" << std::endl;
    (modules_.push_back(Workers()), ...);
  }
};