#ifndef UTILS_H
#define UTILS_H

#include <vector>
typedef struct {
  int x;
  int y;
  int width;
  int height;
} Object;

struct Direction {
  double dx;
  double dy;
  double distance;
};

bool isColliding(Object obj1, Object obj2);

Object createCollisionObj(Object reference, int afterX, int afterY, int width,
                          int height);

void printObject(Object obj);

Direction calculateDistance(Object p1, Object p2);

std::vector<Object> concat(std::vector<Object> v1, std::vector<Object> v2);

#endif