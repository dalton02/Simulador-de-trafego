#include "../../include/core/traffic_light.hpp"
using namespace std::chrono;
using namespace std;

std::deque<TrafficLight> globalLights;

TrafficLight ::TrafficLight(Object obj, int direction, int ticksForToggle,
                            int startWith) {
  this->obj = obj;
  this->green = true;
  this->direction = direction;
  this->ticksForToggle = ticksForToggle;
  this->ticks = startWith;
  this->keepClosed = false;
}

TrafficLight ::~TrafficLight() {}

void TrafficLight ::toggle() { 
  green = !green; 
}