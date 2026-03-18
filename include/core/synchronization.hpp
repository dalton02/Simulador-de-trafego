#ifndef SYNCHRONIZATION_HPP
#define SYNCHRONIZATION_HPP

#include <vector>

class Synchronization {
public:
  explicit Synchronization();
  ~Synchronization();
  void mainLoop();
  void initObjects();

private:
  std::vector<int> cars;
};

#endif