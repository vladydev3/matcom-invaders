#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "../include/enemy.h"
#include "../include/ship.h"
#define MAX_ENEMIES 100

extern volatile int gameRunning;

Enemy* enemyListHead = NULL;

int life = 3;
int score = 0;

void drawEnemies() {
    Enemy* current = enemyListHead;
    while (current != NULL) {
        if (current->isActive) {
            mvprintw(current->y, current->x, "W");
        }
        current = current->next;
    }
}

void checkCollisions() {
    Enemy* current = enemyListHead;
    Enemy* prev = NULL;
    while (current != NULL) {
        if (current->isActive) {
            Shot* shot = shots;
            while (shot != NULL) {
                if (shot->x == current->x && shot->y == current->y) {
                    score += 10;
                    removeEnemy(current);
                    mvaddch(shot->y, shot->x, ' ');
                    break;
                }
                shot = shot->next;
            }
            if (current->isActive && current->x == shipPosition.x && current->y+1 == shipPosition.y) {
                life = 0; // Decrement life if enemy collides with the ship
                removeEnemy(current);
                if (life <= 0) {
                    gameRunning = 0; // End the game if life is zero
                }
            }
        }
        current = current->next;
    }
}

void moveEnemiesDown() {
    Enemy* current = enemyListHead;
    while (current != NULL) {
        if (current->isActive) {
            current->y += 1;
            if (current->y >= LINES ) {
                life--;
                mvaddch(current->y - 1, current->x, ' ');
                removeEnemy(current);
            }
            if (life <= 0) {
                gameRunning = 0;
            }
            mvaddch(current->y - 1, current->x, ' ');
        }
        current = current->next;
    }
}

// Función para agregar un enemigo a la lista
void addEnemy(int x, int y) {
    Enemy* newEnemy = (Enemy*)malloc(sizeof(Enemy));
    if (newEnemy == NULL) {
        // Manejo de error de memoria
        return;
    }
    newEnemy->x = x;
    newEnemy->y = y;
    newEnemy->isActive = 1;
    newEnemy->next = enemyListHead;
    enemyListHead = newEnemy;
}

int activateRandomEnemy() {
    srand(time(NULL)); // Ideally, call this once at the start of your program
    int x = rand() % COLS;
    int y = 1;
    addEnemy(x, y);
    return 1;
}

// Función para eliminar un enemigo de la lista
void removeEnemy(Enemy* enemy) {
    Enemy* temp = enemyListHead;
    mvaddch(enemy->y, enemy->x, ' '); // Clear the enemy from the screen
    Enemy* prev = NULL;
    while (temp != NULL && temp != enemy) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return; // No se encontró el enemigo
    if (prev == NULL) {
        enemyListHead = temp->next;
    } else {
        prev->next = temp->next;
    }
    free(temp);
}