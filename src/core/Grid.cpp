#include "../../include/core/Grid.hpp"

Grid::Grid(int width, int height, int cellSize) 
    : cols(width / cellSize), rows(height / cellSize), cellSize(cellSize) {
    
    // Inicializa a matriz com o tamanho correto
    map.resize(rows);
    for(int r = 0; r < rows; ++r) {
        map[r] = std::vector<Cell>(cols);
    }
    
    initializeRoads();
}

void Grid::initializeRoads() {
    // Posições originais do seu interface.cpp
    int vRoadsX[] = {200, 450, 700, 950};
    int hRoadsY[] = {250, 550};

    // 1. Mapeando as Ruas Verticais (Duas faixas)
    for (int startX : vRoadsX) {
        int leftLaneCol = startX / cellSize;         // Faixa da esquerda
        int rightLaneCol = (startX / cellSize) + 1;  // Faixa da direita

        for (int r = 0; r < rows; ++r) {
            map[r][leftLaneCol].type = CellType::ROAD;
            map[r][leftLaneCol].dir = Direction::UP; // No SFML, Y diminui para cima

            map[r][rightLaneCol].type = CellType::ROAD;
            map[r][rightLaneCol].dir = Direction::DOWN; // Y aumenta para baixo
        }
    }

    // 2. Mapeando as Ruas Horizontais (Duas faixas)
    for (int startY : hRoadsY) {
        int topLaneRow = startY / cellSize;          // Faixa de cima
        int bottomLaneRow = (startY / cellSize) + 1; // Faixa de baixo

        for (int c = 0; c < cols; ++c) {
            // Se já era ROAD (via vertical), virou cruzamento!
            if (map[topLaneRow][c].type == CellType::ROAD) {
                map[topLaneRow][c].type = CellType::INTERSECTION;
                map[topLaneRow][c].dir = Direction::NONE; // Cruzamento permite múltiplas direções
            } else {
                map[topLaneRow][c].type = CellType::ROAD;
                map[topLaneRow][c].dir = Direction::RIGHT; // Cima vai pra direita
            }

            if (map[bottomLaneRow][c].type == CellType::ROAD) {
                map[bottomLaneRow][c].type = CellType::INTERSECTION;
                map[bottomLaneRow][c].dir = Direction::NONE;
            } else {
                map[bottomLaneRow][c].type = CellType::ROAD;
                map[bottomLaneRow][c].dir = Direction::LEFT; // Baixo vai pra esquerda
            }
        }
    }
}

bool Grid::isIntersection(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    return map[r][c].type == CellType::INTERSECTION;
}

bool Grid::isValidMove(int r, int c, Direction moveDir) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    
    CellType type = map[r][c].type;
    if (type == CellType::EMPTY) return false;
    if (type == CellType::INTERSECTION) return true; // Cruzamentos aceitam vindo de qualquer lado
    
    return map[r][c].dir == moveDir; // Garante que não entra na contramão
}

// Tenta trancar a célula (retorna false se outro carro já estiver lá)
bool Grid::tryLockCell(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return false;
    return map[r][c].mtx->try_lock();
}

// Libera a célula para o próximo carro
void Grid::unlockCell(int r, int c) {
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        map[r][c].mtx->unlock();
    }
}