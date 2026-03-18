

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

#include "../utils/utils.hpp"

#include "traffic_light.hpp"

class Car {

private:
  std::mutex mu;

public:
  Object car;

  Object collisorLight;
  Object collisorFront;
  bool active;
  int speedX;
  int speedY;
  int passed;
  bool isAmbulance;
  std::thread thr;
  TrafficLight *currentTrafficLight;
  bool canProcess;
  void calculateCollisor();
  void waitGreenLight();
  void run();
  void standby(std::condition_variable &, std::mutex &, int &);
  void moveToTrafficLight(TrafficLight *);
  bool inFrontOfRedLight();
  bool hasCarInFront();
  TrafficLight *findNearestLight();
  Car(Object object, int speedX, int speedY, bool isAmbulance);

  Car(const Car &) = delete;
  Car &operator=(const Car &) = delete;
};

extern std::vector<std::unique_ptr<Car>> globalCars;