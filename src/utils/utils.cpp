#include "../../include/utils/utils.hpp"
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

bool isCloseToBy(int x, int y, Object obj1, Object obj2) {
  int center1_x = obj1.x + obj1.width / 2;
  int center1_y = obj1.y + obj1.height / 2;

  int center2_x = obj2.x + obj2.width / 2;
  int center2_y = obj2.y + obj2.height / 2;

  int diff_x = center1_x - center2_x;
  int diff_y = center1_y - center2_y;

  bool close_x = (diff_x >= -x && diff_x <= x);
  bool close_y = (diff_y >= -y && diff_y <= y);

  return close_x && close_y;
}

Object createCollisionObj(Object reference, int afterX, int afterY, int width,
                          int height) {

  Object toReturn = {reference.x, reference.y, width, height};
  toReturn.x += afterX;
  toReturn.y += afterY;
  return toReturn;
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