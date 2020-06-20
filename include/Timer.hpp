#include <string>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <chrono>

namespace ScratchPadd {

  #define SCOPED_METHOD_TIMER() ScratchPadd::ScopedTimer scopedTimer(__PRETTY_FUNCTION__)

  class Timer {
    private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime_;
    std::string timerName_;
    public:
    Timer(const std::string &timerName = "DefaultTimer") : timerName_(timerName) {
      
    }

    void start() {
      startTime_ = std::chrono::high_resolution_clock::now();
    }

    void stop() {
      endTime_ = std::chrono::high_resolution_clock::now();
    }

    void stopAndPrint() {
      stop();
      printCurrentDuration();
    }

    double getDurationInSeconds() {
      return std::chrono::duration<double>(endTime_ - startTime_).count();
    }

    void printCurrentDuration() {
      spdlog::info("Timer [{}] ended. Duration = {}",timerName_,getDurationString());
    }
    
    std::string getDurationString() {
      auto timerDuration = endTime_ - startTime_;
      auto timedDays = duration_cast<std::chrono::days>(timerDuration);
      timerDuration -= timedDays;
      auto timedHours = duration_cast<std::chrono::hours>(timerDuration);
      timerDuration -= timedHours;
      auto timedMinutes = duration_cast<std::chrono::minutes>(timerDuration);
      timerDuration -= timedMinutes;
      auto timedSeconds = duration_cast<std::chrono::seconds>(timerDuration);
      timerDuration -= timedSeconds;
      auto timedMilliseconds = duration_cast<std::chrono::milliseconds>(timerDuration);
      return fmt::format("{}h {}m {}s {}ms",timedHours.count(),timedMinutes.count(),timedSeconds.count(),timedMilliseconds.count());
    }
  };

  class ScopedTimer: Timer {
    public:

    ScopedTimer(const std::string timerName = "Default Scoped Timer"): Timer(timerName) {
      start();
    }
    
    ~ScopedTimer() {
      stopAndPrint();
    }
  };
}