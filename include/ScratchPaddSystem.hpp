#pragma once
#include <tuple>
#include <spdlog/spdlog.h>

namespace ScratchPadd {

  template <typename ...Workers>
  class System {
    private:
    static std::tuple<std::unique_ptr<Workers>...> workers_;
    static std::condition_variable scratchPaddSystemConditionVariable_;
    static std::mutex scratchPaddSystemMutex_;
    static bool stopped_;
    public:
    static void instantiate() {
      ((get<std::unique_ptr<Workers>>(workers_) = std::make_unique<Workers>()), ...);
    }

    static void start() { 
      // Start workers on independant threads
      std::apply([](auto const&... worker) {
          (worker->prepare(), ...);
          (worker->start(), ...);
        }, workers_);

      // Start the workers on the main threads

      std::apply([](auto const&... worker) {
        (worker->startingIfMainThread(), ...);
        while (!stopped_) {
          (worker->runIfMainThread(), ...);
        }
        (worker->finishingIfMainThread(), ...);
      },workers_);

      waitForStop();
    }


    static void stop() {
      std::apply([](auto const&... worker) {
          (worker->stop(), ...);
        }, workers_);
        notifyStopped();
    }
    static void waitForStop() {
      std::unique_lock<std::mutex> lk(scratchPaddSystemMutex_);
      std::cout << "Waiting... \n";
      scratchPaddSystemConditionVariable_.wait(lk, []{return stopped_;});
    }

    static void notifyStopped() {
        stopped_ = true;
        scratchPaddSystemConditionVariable_.notify_all();
    }
  };

  template <typename ...Workers>
  std::tuple<std::unique_ptr<Workers>...> System<Workers...>::workers_;

  template <typename ...Workers>
  std::condition_variable System<Workers...>::scratchPaddSystemConditionVariable_;

  template <typename ...Workers>
  std::mutex System<Workers...>::scratchPaddSystemMutex_;

  template <typename ...Workers>
  bool System<Workers...>::stopped_{false};
}

