#ifndef ENEMY_H
#define ENEMY_H
#include <stddef.h>
#include "position.h"

typedef struct Enemy {
    int x, y;
    int isActive;
    struct Enemy* next;
} Enemy;

extern Enemy* enemyListHead; // Puntero a la cabeza de la lista de enemigos

typedef struct EnemyActivationRequest {
    int x, y;
    struct EnemyActivationRequest* next;
} EnemyActivationRequest;

extern EnemyActivationRequest* activationRequestHead;

extern int life;
extern int score;

void drawEnemies();
void checkCollisions();
void moveEnemiesDown();
void removeEnemy(Enemy* enemy);
void activateEnemy(PositionEnemy* pos);
void addEnemy(int x, int y);

#endif // ENEMY_H