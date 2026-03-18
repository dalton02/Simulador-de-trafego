#include "../../include/utils/utils.hpp"
#include <cmath>
#include <iostream>
#include <sstream>

bool isColliding(Object obj1, Object obj2) {
  if (obj1.x + obj1.width <= obj2.x)
    return false;
  if (obj1.x >= obj2.x + obj2.width)
    return false;
  if (obj1.y + obj1.height <= obj2.y)
    return false;
  if (obj1.y >= obj2.y + obj2.height)
    return false;

  return true;
}

Object createCollisionObj(Object reference, int afterX, int afterY, int width,
                          int height) {

  Object toReturn = {reference.x, reference.y, width, height};
  toReturn.x += afterX;
  toReturn.y += afterY;
  return toReturn;
}

std::vector<Object> concat(std::vector<Object> v1, std::vector<Object> v2) {
  v1.insert(v1.end(), v2.begin(), v2.end());
  return v1;
}
Direction calculateDistance(Object p1, Object p2) {
  double dx = p2.x - p1.x;
  double dy = p2.y - p1.y;

  return {dx, dy, std::sqrt(dx * dx + dy * dy)};
}

void printObject(Object obj) {
  std::stringstream ss;
  ss << "{\n";
  ss << "  \"x\": " << obj.x << ",\n";
  ss << "  \"y\": " << obj.y << ",\n";
  ss << "  \"width\": " << obj.width << ",\n";
  ss << "  \"height\": " << obj.height << "\n";
  ss << "}";
  std::cout << ss.str() << "\n";
}