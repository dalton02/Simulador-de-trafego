

#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>

#include "../utils/utils.hpp"

#include "../concurrency/traffic_light.hpp"

class Car {

private:
  std::mutex mu;

public:
  Object *car;
  int speed;
  std::thread thr;
  TrafficLight *currentTrafficLight;
  bool canProcess;

  void waitGreenLight();
  void run();
  void standby(std::condition_variable &, std::mutex &, int &);
  void moveToTrafficLight(TrafficLight *);
  bool inFrontOfRedLight();
  bool hasCarInFront();

  Car(Object *object, int speed, TrafficLight *traffic);

  Car(const Car &) = delete;
  Car &operator=(const Car &) = delete;
};

extern std::deque<Car> globalCars;