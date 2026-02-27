#include "../../include/vehicles/car.hpp"
#include "../../include/utils/utils.hpp"

#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

std::deque<Car> globalCars;
Car ::Car(Object *obj, int speed, TrafficLight *traffic) {
  this->car = obj;
  this->speed = speed;
  this->currentTrafficLight = traffic;
  canProcess = false;
};

void Car::standby(std::condition_variable &ready_variable,
                  std::mutex &ready_mutex, int &done_processing) {

  while (true) {

    std::unique_lock<std::mutex> lock(ready_mutex);

    ready_variable.wait(lock, [this]() {
      if (inFrontOfRedLight()) {
        std::cout << "CARRO DE FRENTE A SINA VERMELHO\n";
      }
      if (hasCarInFront()) {
        std::cout << "CARRO DE FRENTE A OUTRO\n";
      }

      return !inFrontOfRedLight() && !hasCarInFront() && canProcess;
    });

    canProcess = false;

    run();

    lock.unlock();
  }
}

void Car::run() {
  int limit = 10;
  int carSize = 5;
  std::cout << "  CARRO ANDANDO, X:" << car->x << "\n";
  this->car->x++;
}

bool Car::inFrontOfRedLight() {
  int limit = 10;
  return !currentTrafficLight->green &&
         isCloseToBy(limit, limit, *currentTrafficLight->obj,
                     *currentTrafficLight->obj);
}
bool Car::hasCarInFront() {
  for (Car &otherCar : globalCars) {
    if (&otherCar == this) {
      continue;
    }
    if (isCloseToBy(10, 10, *otherCar.car, *this->car)) {
      std::cout << "Parado atras de outro carro...\n";
      return true;
    }
  }
  return false;
}