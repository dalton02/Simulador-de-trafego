#include "../../include/core/car.hpp"
#include "../../include/core/grid.hpp"
#include "../../include/ui/graphics.hpp"
#include <cmath>

std::vector<std::unique_ptr<Car>> globalCars;

TrafficLight *Car::findNearestLight() {

  int direction = 0;

  if (speedY > 0) {
    direction = 1;
  }

  bool movingUp = speedY > 0;
  bool movingDown = speedY < 0;
  bool movingRight = speedX > 0 && std::abs(speedX) >= std::abs(speedY);
  bool movingLeft = speedX < 0 && std::abs(speedX) >= std::abs(speedY);

  TrafficLight *nearest = nullptr;
  double bestDist = 0.0;

  for (TrafficLight &light : globalLights) {
    if (&light == currentTrafficLight || light.direction != direction) {
      continue;
    }

    double dx = light.obj.x - car.x;
    double dy = light.obj.y - car.y;

    bool isAhead = false;

    if (movingUp) {
      isAhead = (dy > 0);
    } else if (movingDown) {
      isAhead = (dy < 0);
    } else if (movingRight) {
      isAhead = (dx > 0);
    } else if (movingLeft) {
      isAhead = (dx < 0);
    }

    if (!isAhead) {
      continue;
    }

    double dist = std::hypot(dx, dy);

    if (nearest == nullptr || dist < bestDist) {
      nearest = &light;
      bestDist = dist;
    }
  }

  return nearest;
}

Car ::Car(Object obj, int speedX, int speedY, bool isAmbulance) {
  this->car = obj;
  this->speedX = speedX;
  this->speedY = speedY;
  this->isAmbulance = isAmbulance;
  this->currentTrafficLight = findNearestLight();
  canProcess = false;

  active = true;
};

void Car::standby(std::condition_variable &ready_variable,
                  std::mutex &ready_mutex) {

  while (true) {

    calculateCollisor();

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

  this->car.x += speedX;
  this->car.y += speedY;

  if (car.x >= WINDOW_WIDTH || car.y >= WINDOW_HEIGHT) {
    active = false;
    return;
  }

  if (currentTrafficLight != nullptr && speedX > 0 &&
      car.x > currentTrafficLight->obj.x) {
    currentTrafficLight = findNearestLight();
  }

  if (currentTrafficLight != nullptr && speedY > 0 &&
      car.y > currentTrafficLight->obj.y) {
    currentTrafficLight = findNearestLight();
  }
}

void Car::calculateCollisor() {
  int toLight = 10;
  int distanceObj = roadManager.size - 5;
  int toFront = 4;
  if (speedX > 0) {
    collisorLight = createCollisionObj(car, car.width + distanceObj, 0, toLight,
                                       car.height);

    collisorFront = createCollisionObj(car, car.width, 0, toFront, car.height);

    return;
  }

  collisorLight =
      createCollisionObj(car, 0, car.height + distanceObj, car.width, toLight);
  collisorFront = createCollisionObj(car, 0, car.height, car.width, toFront);
}

bool Car::inFrontOfRedLight() {
  if (currentTrafficLight == nullptr) {
    return false;
  }
  return !currentTrafficLight->green &&
         isColliding(collisorLight, currentTrafficLight->obj);
}
bool Car::hasCarInFront() {

  for (const auto &otherCar : globalCars) {
    if (otherCar.get() == this) {
      continue;
    }
    bool colliding = isColliding(collisorFront, otherCar->car);
    if (colliding) {
      return true;
    }
  }
  return false;
}