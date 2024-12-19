#pragma once
#include <chrono>
#include <functional>
#include <ostream>
#include <string>
#include <utility>

class timer {
  public:
    timer();
    ~timer();

    virtual void start();
    void         stop();
    int          ms() const;

    friend std::ostream& operator<<(std::ostream& os, const timer& timer);

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
    std::chrono::time_point<std::chrono::high_resolution_clock> _stop;
    bool                                                        stopped = false;
};

class timer_f : public timer {
  public:
    timer_f(const std::string& prefix, const std::function<void()>& f);
    ~timer_f();

  private:
    std::function<void()> f;
    std::string           prefix;

    static std::vector<std::pair<std::string, int>> pump;
    static int                                      ref_cnt;
};