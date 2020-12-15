#ifndef SRC_POM_RENDERER_TIMER_HPP
#define SRC_POM_RENDERER_TIMER_HPP

#include <chrono>

class Timer {
 public:
  [[nodiscard]] auto SecondPassed() noexcept -> bool {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - last_second_);
    if (ms.count() >= kOneSecondInMS) {
      last_second_ = now;
      return true;
    }
    return false;
  }

  [[nodiscard]] auto TickPassed() noexcept -> bool {
    auto now = std::chrono::system_clock::now();
    auto ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(now - last_tick_);
    if (ms.count() >= kOneSecondInMS * tick_rate_) {
      last_tick_ = now;
      return true;
    }
    return false;
  }

 private:
  static constexpr int kOneSecondInMS = 1000;
  static constexpr float k60FPS = 1.0F / 60;
  static constexpr float k30FPS = 1.0F / 30;

  std::chrono::system_clock::time_point const now_ =
      std::chrono::system_clock::now();

  float tick_rate_{k60FPS};
  std::chrono::system_clock::time_point last_tick_{now_};
  std::chrono::system_clock::time_point last_second_{now_};
};

#endif  // SRC_POM_RENDERER_TIMER_HPP
