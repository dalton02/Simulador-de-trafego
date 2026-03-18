#ifndef SYNCHRONIZATION_HPP
#define SYNCHRONIZATION_HPP

#include <vector>
#define CLOCK 30

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