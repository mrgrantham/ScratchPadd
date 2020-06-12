#pragma once

#include <unordered_map>// #include <type_traits>
#include <memory>
#include <thread>
// #include <cstddef>

// template <typename T, typename... Ts>
// constexpr std::size_t Index_v = Index<T, Ts...>::value;

enum class WorkerSetting {
  worker_off,
  worker_on,
  debug_off,
};

union WorkerSettingValue {
  double decimal[4];
  bool boolean;
  int64_t integer[4];
  char message[20];
};

class BaseWorker;

class WorkerSystemConfiguration {
public:
std::unordered_map<WorkerSetting,WorkerSettingValue> configurationMap_;
};

template <typename ...Workers>
class WorkerDispatcher {
private:

  const std::chrono::seconds watchdogInterval_{1};

  std::vector<BaseWorker*> workers_;
  std::thread dispatcherThread_;
  bool on_{true};


  void printWorkers() {
    ((std::cout << " name type: " << typeid(Workers).name() << std::endl), ...);
  }

  void instantiate() {
    (workers_.push_back(new Workers(*this)), ...);
  }

  void prepare() {
    int i = 0;
    (std::invoke(&Workers::prepare,dynamic_cast<Workers*>(workers_[i++])), ...);
  }

  void startWorkers() {
    int i = 0;
    (std::invoke(&Workers::start,dynamic_cast<Workers*>(workers_[i++])), ...);
  }

  public:
  WorkerDispatcher() {
    std::cout << "Starting Dispatcher" << std::endl;
    printWorkers();
    instantiate();
  }


  void run() {
    std::cout << "starting WorkerDispatcher" << std::endl;
    std::cout << "created vector of workers" << std::endl;
    // ((std::cout << " name object: " << typeid(Workers).name() << std::endl), ...);
    // (dynamic_cast<Workers>(workers_[i++]).run(), ...);
    while (on_) {
      std::this_thread::sleep_for(watchdogInterval_);
    }
  }
  void start() {
    dispatcherThread_ = std::thread(&WorkerDispatcher::run,this);
    prepare();
    startWorkers();
  }

  void stop() {
    int i = 0;
    (std::invoke(&Workers::stop,dynamic_cast<Workers*>(workers_[i++])), ...);
    on_ = false;
  }

  void config(WorkerSystemConfiguration &configuration) {
    int i = 0;
    (std::invoke(&Workers::config,dynamic_cast<Workers*>(workers_[i++],configuration)), ...);
  }

  void join() {
    if (dispatcherThread_.joinable()) {
      dispatcherThread_.join();
    }
  }

  template <typename Data, typename ...ReceivingWorker>
  void sendData(Data &data) {
        int i = 0;
    (std::invoke(&ReceivingWorker::receiveData,dynamic_cast<Workers*>(workers_[i++].get(),data)), ...);
  }


};

class TestWorker1;
class TestWorker2;
class TestWorker3;

#define Dispatcher WorkerDispatcher<TestWorker1, TestWorker2, TestWorker3>
