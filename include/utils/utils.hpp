#ifndef UTILS_H
#define UTILS_H

typedef struct {
  int x;
  int y;
  int width;
  int height;
} Object;

bool isColliding(Object obj1, Object obj2);

/// Retorna se um objeto está proximo do outro  por x metros ou y metros
bool isCloseToBy(int x, int y, Object obj1, Object obj2);

Object createCollisionObj(Object reference, int afterX, int afterY, int width,
                          int height);

void printObject(Object obj);

#endif