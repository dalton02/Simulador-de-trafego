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

  initObjects();

  std::mutex clockMutex;
  std::condition_variable clockCondition;

  int ticks = 0;
  int workersProcessed = 0;

  const auto tick_duration = std::chrono::milliseconds(40);

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

    draw();

    for (const auto &car : globalCars) {
      car->canProcess = true;
    }

    for (TrafficLight &traffic : globalLights) {
      traffic.keepClosed = false;
    }

    for (TrafficLight &traffic : globalLights) {

      bool hasAmbulance = false;

      for (const auto &car : globalCars) {
        // ignora carrito que não pertence ao semaforo ou ja passou da tela
        if (car->currentTrafficLight != &traffic || !car->active) {
          continue;
        }

        // pegamos o semaforo vertical ou horizontal do nosos cruzamento
        TrafficLight &closest = globalLights[traffic.closest];

        // se o carro é ambulancia, mantem o sinal proximo fechado, e o nosso
        // aberto
        if (car->isAmbulance) {
          hasAmbulance = true;
          closest.keepClosed = true;
        }
      }

      traffic.process(clockMutex, clockCondition, hasAmbulance);
    }

    clockCondition.notify_all();

    std::this_thread::sleep_for(tick_duration);
  }
}

// Area de inicialização e testes
void Synchronization::initObjects() {
  load();

  roadManager.generateIntersections();

  std::vector<Object> &crossings = roadManager.objects;
  int lightSize = 20;

  int sync = 80;

  int count = 0;

  for (Object &cross : crossings) {
    int centerX = cross.x + lightSize / 4;
    int centerY = cross.y + lightSize / 4;

    Object lightObj = Object{centerX, centerY, lightSize, lightSize};

    globalLights.emplace_back(lightObj, 0, sync, sync - 1);
    globalLights.emplace_back(lightObj, 1, sync, 0);

    globalLights[count].closest = count + 1;
    globalLights[count + 1].closest = count;

    count += 2;
  }

  std::vector<Object> carsX;
  carsX = concat(carsX, roadManager.genObjectsFromRoad(0, 0, 1, 120, 40, 40));
  carsX = concat(carsX, roadManager.genObjectsFromRoad(0, 1, 3, 120, 40, 40));
  carsX = concat(carsX, roadManager.genObjectsFromRoad(0, 2, 2, 120, 40, 40));

  std::vector<Object> carsY;
  carsY = concat(carsY, roadManager.genObjectsFromRoad(1, 0, 2, 140, 40, 40));
  carsY = concat(carsY, roadManager.genObjectsFromRoad(1, 1, 2, 140, 40, 40));
  carsY = concat(carsY, roadManager.genObjectsFromRoad(1, 2, 2, 130, 40, 40));

  count = 0;
  int speed = 1;

  int values[] = {1, 2, 4};

  for (Object c : carsX) {

    int isAmbulance = false;

    if (count == 4) {
      isAmbulance = true;
    }

    speed = values[rand() % 3];

    globalCars.push_back(std::make_unique<Car>(c, speed, 0, isAmbulance));
    count++;
  }
  for (Object c : carsY) {

    speed = values[rand() % 3];

    globalCars.push_back(std::make_unique<Car>(c, 0, speed, false));
  }

  std::cout << "Inicializados " << globalCars.size() << " carros\n";
}
