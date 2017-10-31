#include <chrono>
#include <functional>
#include <iostream>
#include <string>

class Timer {
private:
  std::chrono::high_resolution_clock::time_point start;
  std::string name;

public:
  Timer(const std::string &n)
      : start(std::chrono::high_resolution_clock::now()), name(n) {}

  long elapsed() {
    auto end = std::chrono::high_resolution_clock::now();
    return
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
  }

  ~Timer() {
    std::cout << "Time of " << name << " : " << elapsed()  << "ns."<< std::endl;
  }
};
