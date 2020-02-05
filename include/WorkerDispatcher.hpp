#include <BaseWorker.hpp>
// #include <type_traits>
// #include <cstddef>

// template <typename T, typename... Ts>
// constexpr std::size_t Index_v = Index<T, Ts...>::value;

template <typename ...Workers>
class WorkerDispatcher {
  std::vector<std::unique_ptr<BaseWorker>> workers_;
  std::thread dispatcherThread_;
  bool on_{true};
  public:
  WorkerDispatcher() {
    std::cout << "Starting Dispatcher" << std::endl;
  }

  void run() {
    std::cout << "starting WorkerDispatcher" << std::endl;
    (workers_.push_back(std::make_unique<Workers>()), ...);
    std::cout << "created vector of workers" << std::endl;
    ((std::cout << " name type: " << typeid(Workers).name() << std::endl), ...);
    // ((std::cout << " name object: " << typeid(Workers).name() << std::endl), ...);
    // (dynamic_cast<Workers>(workers_[i++]).run(), ...);
    int i = 0;
    (std::invoke(&Workers::prepare,dynamic_cast<Workers*>(workers_[i++].get())), ...);
    i = 0;
    (std::invoke(&Workers::start,dynamic_cast<Workers*>(workers_[i++].get())), ...);
    while (on_) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
  void start() {
    dispatcherThread_ = std::thread(&WorkerDispatcher::run,this);
  }

  void stop() {
    int i = 0;
    (std::invoke(&Workers::stop,dynamic_cast<Workers*>(workers_[i++].get())), ...);
    on_ = false;
    join();
  }

  void join() {
    if (dispatcherThread_.joinable()) {
      dispatcherThread_.join();
    }
  }

  void prepareWorkers() {
    for (auto &worker: workers_) {
      worker->prepare();
    }
  }
  void beginRunLoop() {
    for (auto &worker: workers_) {
      worker->run();
    }
  }

  void runOnAllWorkers(WorkerMethod method) {
    for (auto &worker : workers_) {

      std::invoke(method, worker);
    }
  }
};