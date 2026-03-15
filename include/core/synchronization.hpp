#ifndef SYNCHRONIZATION_HPP
#define SYNCHRONIZATION_HPP

#include "traffic_light.hpp"
#include <thread>
#include <vector>

class Synchronization {
public:
  explicit Synchronization();
  ~Synchronization();
  void mainLoop();

private:
  std::vector<int> cars;
};

#endif