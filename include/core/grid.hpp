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
};

#endif