#include "../../include/vehicles/car.hpp"
#include "../../include/utils/utils.hpp"
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>

std::deque<Car> globalCars;
Car ::Car(Object *obj, int speedX, int speedY, TrafficLight *traffic) {
  this->car = obj;
  this->speedX = speedX;
  this->speedY = speedY;
  this->currentTrafficLight = traffic;
  canProcess = false;
};

void Car::standby(std::condition_variable &ready_variable,
                  std::mutex &ready_mutex, int &done_processing) {

  while (true) {

    std::unique_lock<std::mutex> lock(ready_mutex);

    ready_variable.wait(lock, [this]() {
      if (inFrontOfRedLight()) {
        std::cout << "CARRO DE FRENTE A SINAL VERMELHO\n";
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
  this->car->x += speedX;
}

bool Car::inFrontOfRedLight() {
  int limit = 5;

  if (speedX > 0) {
    Object collisionX =
        createCollisionObj(*car, car->width, 0, limit + speedX, car->height);

    return !currentTrafficLight->green &&
           isColliding(collisionX, *currentTrafficLight->obj);
  }

  Object collisionY =
      createCollisionObj(*car, 0, car->height, car->width, limit + speedY);
  return !currentTrafficLight->green &&
         isColliding(collisionY, *currentTrafficLight->obj);
}
bool Car::hasCarInFront() {
  int limit = 1;
  for (Car &otherCar : globalCars) {
    if (&otherCar == this) {
      continue;
    }
    if (speedX > 0) {
      Object collisionX =
          createCollisionObj(*car, car->width, 0, limit + speedX, car->height);

      return isColliding(collisionX, *otherCar.car);
    }

    Object collisionY =
        createCollisionObj(*car, 0, car->height, car->width, limit + speedY);
    return isColliding(collisionY, *otherCar.car);
  }
  return false;
}