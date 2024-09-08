#include <stdio.h>
#include <stdlib.h>
#ifndef POSITION_H
#define POSITION_H

// Estructura para manejar la lista LRU de posiciones de aterrizaje
typedef struct PositionEnemy {
    int x;
    int y;
    struct PositionEnemy* next; // Para construir la lista LRU
} PositionEnemy;
// Lista LRU de posiciones donde pueden aterrizar los enemigos


void initializeLandingPositions(int numPositions);
void moveToHead(PositionEnemy* pos);
void activateEnemyLRU();
void freeLandingPositions();

#endif // POSITION_H
