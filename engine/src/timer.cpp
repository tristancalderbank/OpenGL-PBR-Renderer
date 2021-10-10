#include "timer.h"

Timer::Timer() {
    start = std::chrono::steady_clock::now();
}

void Timer::logDifference(const std::string &logText) {
    auto stop = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << logText << " [" << diff << " ms]" << std::endl;
}