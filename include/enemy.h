#ifndef ENEMY_H
#define ENEMY_H
#include <stddef.h>

typedef struct Enemy {
    int x, y;
    int isActive;
    struct Enemy* next;
} Enemy;

extern Enemy* enemyListHead; // Puntero a la cabeza de la lista de enemigos

extern int life;
extern int score;

void drawEnemies();
void checkCollisions();
void moveEnemiesDown();
int activateRandomEnemy();
void removeEnemy(Enemy* enemy);
void addEnemy(int x, int y);

#endif // ENEMY_H