#include "../include/core/synchronization.hpp"

int main() {
  Synchronization *sync = new Synchronization();
  sync->mainLoop();
  return 0;
}
