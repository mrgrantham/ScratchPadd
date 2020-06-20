#pragma once
#include <tuple>
#include <spdlog/spdlog.h>

namespace ScratchPadd {

  template <typename ...Workers>
  class System {
    private:
    static std::tuple<std::unique_ptr<Workers>...> workers_;
    public:
    static void instantiate() {
      ((get<std::unique_ptr<Workers>>(workers_) = std::make_unique<Workers>()), ...);
    }
    static void start() {

      std::apply([](auto const&... worker) {
          (worker->prepare(), ...);
          (worker->start(), ...);
        }, workers_);
    }
    static void stop() {
      std::apply([](auto const&... worker) {
          (worker->stop(), ...);
        }, workers_);
    }
  };

  template <typename ...Workers>
  std::tuple<std::unique_ptr<Workers>...> System<Workers...>::workers_;
}