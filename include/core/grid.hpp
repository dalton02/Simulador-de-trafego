// grid.hpp
#ifndef GRID_HPP
#define GRID_HPP

#include <cstdint>
#include <iostream>
#include <vector>

#include "../ui/graphics.hpp"
#include "../utils/utils.hpp"

class RoadGrid {
public:
  std::vector<Object> objects;

  int size = 30;
  int spacing = 300;
  int qtd = 12;
  int limit = 4;
  int initXSpace = 150;
  int initYSpace = 90;

  RoadGrid() = default;

  float positionYFromRoad(int n) const { return n * spacing + initXSpace; }

  float positionXFromRoad(int n) const { return n * spacing + initYSpace; }

  std::vector<Object> genObjectsFromRoad(int pos, int from, int _qtd,
                                         int _spacing, int width, int height) {
    std::vector<Object> objs;

    // eixo X
    if (pos == 0) {
      int startY = from * spacing + initYSpace;
      for (int i = 0; i < _qtd; i++) {
        Object obj = Object(i * -_spacing, startY, width, height);
        objs.push_back(obj);
      }
    } else {
      int startX = from * spacing + initXSpace;
      for (int i = 0; i < _qtd; i++) {
        Object obj = Object(startX, i * -_spacing, width, height);
        objs.push_back(obj);
      }
    }

    return objs;
  }

  void generateIntersections() {

    for (int i = 0; i < qtd; i++) {
      int linha = i / limit;
      int coluna = i % limit;

      int startX = coluna * spacing + initXSpace;
      int startY = linha * spacing + initYSpace;
      Object obj{startX, startY, size, size};

      objects.push_back(obj);
    }
  }

  std::vector<Object> getRoads() {

    std::vector<Object> objs;

    int totalRoadsY = limit;
    int totalRoadsX = qtd / limit;

    for (int i = 0; i < totalRoadsY; i++) {
      int start = i * spacing + initXSpace;

      Object obj{start, 0, size, WINDOW_HEIGHT};
      objs.push_back(obj);
    }

    for (int i = 0; i < totalRoadsX; i++) {
      int start = i * spacing + initYSpace;

      Object obj{0, start, WINDOW_WIDTH, size};
      objs.push_back(obj);
    }
    return objs;
  }
};

extern RoadGrid roadManager;

#endif