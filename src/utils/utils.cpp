#include "../../include/utils/utils.hpp"

// As funções antigas de colisão por pixel (isColliding, isCloseToBy, etc.)
// foram removidas. 
// Agora o Grid (Malha Viária) gerencia as colisões lógicas de forma segura
// usando mutexes em cada célula de 25x25, cumprindo a regra de 
// "impenetrabilidade da matéria" exigida no trabalho.