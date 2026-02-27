#include "../../include/concurrency/traffic_light.hpp"
#include "../../include/utils/utils.hpp"

#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>

using namespace std::chrono;
using namespace std;

std::deque<TrafficLight> globalLights;

TrafficLight ::TrafficLight(Object *obj, int ticksForToggle) {
  this->obj = obj;
  this->green = true;
  this->ticksForToggle = ticksForToggle;
}

TrafficLight ::~TrafficLight() {}

void TrafficLight ::process(mutex &mu, condition_variable &cv) {

  ++ticks;

  if (ticks % ticksForToggle == 0) {
    cout << "\nTOGGLING LIGHT\n";
    unique_lock<mutex> lock(mu);
    toggle();
  }
}

void TrafficLight ::toggle() { green = !green; }
