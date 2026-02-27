#include "../../include/concurrency/synchronization.hpp"
#include "../../include/utils/utils.hpp"
#include "../../include/vehicles/car.hpp"

#include <cmath>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

Synchronization::Synchronization() {
  Object *obj = new Object{100, 50, 40, 40};
}

void Synchronization::mainLoop() {

  std::mutex clockMutex;
  std::condition_variable clockCondition;

  int ticks = 0;
  int workersProcessed = 0;
  bool canProcess = false;

  const auto tick_duration = std::chrono::milliseconds(400);

  Object *light1 = new Object{25, 0, 10, 10};

  Object *car1 = new Object{0, 0, 10, 10};
  Object *car2 = new Object{35, 0, 10, 10};

  TrafficLight &trafficLight1 = globalLights.emplace_back(light1, 4);

  globalCars.emplace_back(car1, 2, &trafficLight1);
  globalCars.emplace_back(car2, 2, &trafficLight1);

  for (Car &car : globalCars) {
    car.thr =
        std::thread([&car, &clockCondition, &clockMutex, &workersProcessed]() {
          car.standby(clockCondition, clockMutex, workersProcessed);
        });
  }

  for (Car &car : globalCars) {
    car.thr.detach();
  }

  while (true) {
    for (Car &car : globalCars) {
      car.canProcess = true;
    }
    for (TrafficLight &traffic : globalLights) {
      traffic.process(clockMutex, clockCondition);
    }

    clockCondition.notify_all();

    std::this_thread::sleep_for(tick_duration);
  }
}
