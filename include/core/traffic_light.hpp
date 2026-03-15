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
  Object *obj;
  std::condition_variable cv;
  std::mutex mu;
  TrafficLight(Object *obj, int ticksForToggle);
  ~TrafficLight();

  void process(std::mutex &, std::condition_variable &);
};

extern std::deque<TrafficLight> globalLights;

#endif