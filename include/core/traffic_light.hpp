#ifndef CLOCK_HPP
#define CLOCK_HPP
#include "../utils/utils.hpp"

#include <condition_variable>
#include <deque>
#include <mutex>
#include <semaphore>

class TrafficLight {

private:
  int ticksForToggle;
  int ticks;
  void toggle();

public:
  bool green;
  int direction;
  bool keepClosed;
  int closest;
  Object obj;
  std::condition_variable cv;
  std::mutex mu;
  TrafficLight(Object obj, int direction, int ticksForToggle, int startWith);
  ~TrafficLight();

  void process(std::mutex &, std::condition_variable &, bool);
};

extern std::deque<TrafficLight> globalLights;

#endif