#pragma once

#include <string>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <string>

namespace ScratchPadd {

  #define SCOPED_METHOD_TIMER() ScratchPadd::ScopedTimer scopedTimer(__PRETTY_FUNCTION__)

  class Timer {
    private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    std::chrono::time_point<std::chrono::high_resolution_clock> endTime_;
    std::string timerName_;
    std::vector <std::chrono::duration<double>> intervals_;
    public:
    Timer(const std::string &timerName = "DefaultTimer") : timerName_(timerName) {
      
    }

    void setTimerName(const std::string &name) {
      timerName_ = name;
    }

    void start() {
      startTime_ = std::chrono::high_resolution_clock::now();
      endTime_ = startTime_; // This is so getting intervals will work are well
    }

    void markTime() {
      endTime_ = std::chrono::high_resolution_clock::now();
    }

    void markTimeAndPrint() {
      markTime();
      printDuration();
    }

    double getDurationInSeconds() {
      return std::chrono::duration<double>(endTime_ - startTime_).count();
    }
    double getIntervalInSeconds() {
      return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - endTime_).count();
    }

    void markInterval() {
      auto newIntervalEndTime = std::chrono::high_resolution_clock::now();
      auto interval = newIntervalEndTime - endTime_;
      intervals_.push_back(interval);
      endTime_ = newIntervalEndTime;
    }

    double markAndGetIntervalInSeconds() {
      auto newIntervalEndTime = std::chrono::high_resolution_clock::now();
      auto interval = newIntervalEndTime - endTime_;
      intervals_.push_back(interval);
      double lastInterval = std::chrono::duration<double>(interval).count();
      endTime_ = newIntervalEndTime;
      return lastInterval;
    }
    

    void markAndPrintInterval() {
      auto newIntervalEndTime = std::chrono::high_resolution_clock::now();
      auto interval = newIntervalEndTime - endTime_;
      intervals_.push_back(interval);
      spdlog::info("Timer [{}] Interval {}",timerName_, formatIntervalToString(interval));
      endTime_ = newIntervalEndTime;
    }

    void printAverageInterval() {
      std::chrono::duration<double> interval_sum;
      for (auto &interval: intervals_) {
        interval_sum += interval;
      }
      auto interval_avg = interval_sum / intervals_.size();
      spdlog::info("Timer [{}] Size {} Interval Avg {}",timerName_, intervals_.size(), formatIntervalToString(interval_avg));
    }

    void printCurrentDuration() {
      spdlog::info("Timer [{}] Current Duration = {}",timerName_,getCurrentDurationString());
    }

    void printDuration() {
        spdlog::info("Timer [{}] Duration = {}",timerName_,getDurationString());
    }

    std::string getDurationString() {
      return getIntervalString(endTime_,startTime_);
    }

    std::string getCurrentDurationString() {
      return getIntervalString(std::chrono::high_resolution_clock::now(),startTime_);
    }

    template <typename Interval>
    std::string formatIntervalToString(Interval interval) {
      auto timedDays = duration_cast<std::chrono::days>(interval);
      interval -= timedDays;
      auto timedHours = duration_cast<std::chrono::hours>(interval);
      interval -= timedHours;
      auto timedMinutes = duration_cast<std::chrono::minutes>(interval);
      interval -= timedMinutes;
      auto timedSeconds = duration_cast<std::chrono::seconds>(interval);
      interval -= timedSeconds;
      auto timedMilliseconds = duration_cast<std::chrono::milliseconds>(interval);
      return fmt::format("{}h {}m {}s {}ms",timedHours.count(),timedMinutes.count(),timedSeconds.count(),timedMilliseconds.count());
    }
    
    template <typename Duration>
    std::string getIntervalString(Duration endTime, Duration startTime) {
      auto timerDuration = endTime - startTime;
      return formatIntervalToString(timerDuration);
    }
  };

  class ScopedTimer: Timer {
    public:

    ScopedTimer(const std::string timerName = "Default Scoped Timer"): Timer(timerName) {
      start();
    }
    
    ~ScopedTimer() {
      markTimeAndPrint();
    }
  };
}