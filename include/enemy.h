#ifndef ENEMY_H
#define ENEMY_H

typedef struct {
    int x, y;
    int isActive;
} Enemy;

extern Enemy enemies[10];
extern int life;
extern int score;

void drawEnemies();
void checkCollisions();
void moveEnemiesDown();
void cleanupEnemy(int i);
int activateRandomEnemy();


#endif // ENEMY_H