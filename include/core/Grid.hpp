#pragma once
#include <vector>
#include <mutex>
#include <memory>

// Direções permitidas na célula para forçar a mão da via
enum class Direction { NONE, UP, DOWN, LEFT, RIGHT };
enum class CellType { EMPTY, ROAD, INTERSECTION };

struct Cell {
    CellType type = CellType::EMPTY;
    Direction dir = Direction::NONE;
    
    // Cada pedaço do mapa tem sua própria "tranca" (mutex).
    // Isso garante a regra 1 do seu PDF: Impenetrabilidade da matéria.
    std::unique_ptr<std::mutex> mtx;

    Cell() : mtx(std::make_unique<std::mutex>()) {}
};

class Grid {
private:
    int cols, rows;
    int cellSize;
    std::vector<std::vector<Cell>> map;

public:
    Grid(int width, int height, int cellSize);
    
    void initializeRoads();
    
    // Funções para as threads dos carros usarem
    bool isIntersection(int r, int c);
    bool isValidMove(int r, int c, Direction moveDir);
    
    // Controle de concorrência: tenta ocupar a célula
    bool tryLockCell(int r, int c);
    void unlockCell(int r, int c);
    
    // Conversores de Pixel (SFML) para Índice da Matriz
    int getCol(float x) const { return static_cast<int>(x) / cellSize; }
    int getRow(float y) const { return static_cast<int>(y) / cellSize; }
    
    // Pegar o centro da célula em pixels para desenhar o carro centralizado
    float getPixelX(int c) const { return (c * cellSize) + (cellSize / 2.0f); }
    float getPixelY(int r) const { return (r * cellSize) + (cellSize / 2.0f); }
};