#pragma once

namespace ScratchPadd {
class EventTimer {
  bool should_stop_{false};
  public:
  template <typename Event>
  void startRepeatingEvent(const Event &event, int interval_between_events) {
    auto timer_thread = std::thread([=]{
      while (should_stop_ == false) {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_between_events));
        if (should_stop_) break;
        event();
      };
    });
    timer_thread.detach();
  }

  template <typename Event>
  void startTimedEvent(const Event &event, int delay_until_event) {
    auto timer_thread = std::thread([=]{
      if (should_stop_) return;
      std::this_thread::sleep_for(std::chrono::milliseconds(delay_until_event));
      if (should_stop_) return;
      event();
    });
    timer_thread.detach();
  }

  void stop() {
    should_stop_ = true;
  }
};
}