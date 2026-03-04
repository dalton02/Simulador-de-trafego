#include "../../include/vehicles/car.hpp"
#include "../../include/utils/utils.hpp"
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>

std::deque<Car> globalCars;

Car ::Car(Object *obj, int speedX, int speedY, TrafficLight *traffic,
          bool isAmbulance) {
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
  this->car->x += speedX;

  int direction = 0;
  if (speedY > 0) {
    direction = 1;
  }

  if (car->x > currentTrafficLight->obj->x) {

    TrafficLight *nearestTrafficLight = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (TrafficLight &traffic : globalLights) {
      if (&traffic == this->currentTrafficLight ||
          traffic.direction != direction) {
        continue;
      }

      if (traffic.obj->x > car->x) {
        float distance = traffic.obj->x - car->x;
        if (distance < minDistance) {
          minDistance = distance;
          nearestTrafficLight = &traffic;
        }
      }
    }

    if (nearestTrafficLight != nullptr) {
      std::cout << "  TROCANDO DE SINAL" << "\n";
      this->currentTrafficLight = nearestTrafficLight;
    }
  }
}

bool Car::inFrontOfRedLight() {
  int limit = 5;

  if (currentTrafficLight == nullptr) {
    return false;
  }

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