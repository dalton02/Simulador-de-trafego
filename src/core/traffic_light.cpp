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

void TrafficLight ::process(mutex &mu, condition_variable &cv, bool ambulance) {

  if (keepClosed) {
    if (green) {
      unique_lock<mutex> lock(mu);
      toggle();
    }
    ticks = 0;
    return;
  }

  if (ambulance) {
    if (!green) {
      unique_lock<mutex> lock(mu);
      toggle();
    }
    ticks = 0;
    return;
  }

  int compensation = 0;

  // simulando compensação para dar um tempinho antes do semaforo vizinho ficar
  // verde.
  if (!green) {
    compensation = 10;
  }
  ++ticks;

  if (ticks >= ticksForToggle + compensation) {
    unique_lock<mutex> lock(mu);
    toggle();
    ticks = 0;
  }
}

void TrafficLight ::toggle() { green = !green; }
