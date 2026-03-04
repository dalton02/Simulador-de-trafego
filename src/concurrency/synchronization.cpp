#include "../../include/concurrency/synchronization.hpp"
#include "../../include/utils/utils.hpp"
#include "../../include/vehicles/car.hpp"
#include <SFML/Graphics.hpp>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <cmath>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

using namespace std;

Synchronization::Synchronization() {
  Object *obj = new Object{100, 50, 40, 40};
}

void Synchronization::mainLoop() {

  sf::RenderWindow window(sf::VideoMode({800, 600}), "Retangulos Simples");

  std::mutex clockMutex;
  std::condition_variable clockCondition;

  int ticks = 0;
  int workersProcessed = 0;
  bool canProcess = false;

  const auto tick_duration = std::chrono::milliseconds(300);

  Object *light1 = new Object{65, 30, 5, 10};
  Object *light2 = new Object{125, 30, 5, 10};

  Object *car1 = new Object{0, 30, 10, 10};
  Object *car2 = new Object{11, 30, 10, 10};

  TrafficLight &trafficLight1 = globalLights.emplace_back(light1, 0, 4);
  globalLights.emplace_back(light2, 0, 4);

  globalCars.emplace_back(car1, 10, 0, &trafficLight1, false);
  globalCars.emplace_back(car2, 6, 0, &trafficLight1, false);

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

    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);

    for (Car &car : globalCars) {
      sf::RectangleShape rect(sf::Vector2f(car.car->width, car.car->height));

      float pixelX = car.car->x * 1.f;
      float pixelY = car.car->y * 1.f;

      rect.setPosition({pixelX, pixelY});
      rect.setFillColor(sf::Color::Green);
      rect.setOutlineColor(sf::Color::White);

      window.draw(rect);
    }

    for (TrafficLight &light : globalLights) {
      sf::RectangleShape rect(
          sf::Vector2f(light.obj->width, light.obj->height));

      float pixelX = light.obj->x * 1.f;
      float pixelY = light.obj->y * 1.f;

      sf::Color color = sf::Color::Red;
      if (light.green) {
        color = sf::Color::Green;
      }

      rect.setPosition({pixelX, pixelY});
      rect.setFillColor(color);
      rect.setOutlineColor(sf::Color::White);
      rect.setOutlineThickness(2);

      window.draw(rect);
    }

    window.display();

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
