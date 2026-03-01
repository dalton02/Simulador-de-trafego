#include "../include/concurrency/synchronization.hpp"

#include <cstdio>
#include <iostream>

int main() {
  Synchronization *sync = new Synchronization();
  sync->mainLoop();
  return 0;
}
