#include "../../include/core/synchronization.hpp"
#include "../../include/core/car.hpp"
#include "../../include/core/grid.hpp"
#include "../../include/ui/graphics.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/VideoMode.hpp>

#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

Synchronization::Synchronization() {
  Object *obj = new Object{100, 50, 40, 40};
}

void Synchronization::mainLoop() {

  std::mutex clockMutex;
  std::condition_variable clockCondition;

  int ticks = 0;
  int workersProcessed = 0;

  const auto tick_duration = std::chrono::milliseconds(40);

  Object *light1 = new Object{25, 0, 5, 10};

  Object *car1 = new Object{6, 0, 10, 10};
  Object *car2 = new Object{35, 0, 10, 10};

  TrafficLight &trafficLight1 = globalLights.emplace_back(light1, 4);

  globalCars.emplace_back(car1, 2, 0, &trafficLight1);
  //  globalCars.emplace_back(car2, 2, &trafficLight1);

  for (const auto &car : globalCars) {
    car->thr =
        std::thread([&car, &clockCondition, &clockMutex, &workersProcessed]() {
          car->standby(clockCondition, clockMutex, workersProcessed);
        });
  }

  for (const auto &car : globalCars) {
    car.thr.detach();
  }

  while (true) {

    for (const auto &car : globalCars) {
      car.canProcess = true;
    }

    for (TrafficLight &traffic : globalLights) {
      traffic.process(clockMutex, clockCondition);
    }

    clockCondition.notify_all();

    std::this_thread::sleep_for(tick_duration);
  }
}
