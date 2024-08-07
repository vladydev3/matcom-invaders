#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "../include/enemy.h"
#include "../include/ship.h"
#define MAX_ENEMIES 10

extern volatile int gameRunning;
Enemy enemies[MAX_ENEMIES];

int enemyQueue[MAX_ENEMIES];
int front = -1;
int rear = -1;

int life = 3;
int score = 0;

int isQueueEmpty() {
    return front == -1;
}


// Function to add an element to the queue
void enqueue(int index) {
    if (rear == -1) {
        front = rear = 0;
    } else {
        rear = (rear + 1) % MAX_ENEMIES;
    }
    enemyQueue[rear] = index;
}

// Function to remove and return the front element from the queue
int dequeue() {
    if (isQueueEmpty()) {
        return -1; // Queue is empty
    }
    int index = enemyQueue[front];
    if (front == rear) {
        front = rear = -1; // Queue becomes empty
    } else {
        front = (front + 1) % MAX_ENEMIES;
    }
    return index;
}

void drawEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isActive) {
            mvprintw(enemies[i].y, enemies[i].x, "W");
        }
    }
}

void checkCollisions() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].isActive) continue;

        for (int j = 0; j < shotsCount; j++) {
            if (shots[j].isActive && shots[j].x == enemies[i].x && shots[j].y == enemies[i].y) {
                enemies[i].isActive = 0; 
                shots[j].isActive = 0; // Deactivate the shot
                cleanupShots();
                cleanupEnemy(i);
                score += 10;
                // Optionally, add code to handle score or effects
            }
        }
        if (enemies[i].isActive && enemies[i].x == shipPosition.x && enemies[i].y == shipPosition.y) {
            life--; // Decrement life if enemy collides with the ship
            cleanupEnemy(i);
            if (life <= 0) {
                gameRunning = 0; // End the game if life is zero
            }
        }
    }
}

void moveEnemiesDown() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (enemies[i].isActive) {
            enemies[i].y += 1; // Move enemy down by increasing y coordinate
            if (enemies[i].y >= LINES - 2) {
                life--; // Decrement life if enemy reaches the bottom
                cleanupEnemy(i);
            }
            if (life <= 0) {
                gameRunning = 0; // End the game if life is zero
            }   
            mvaddch(enemies[i].y - 1, enemies[i].x, ' ');
        }
    }
}

void cleanupEnemy(int i) {
    enemies[i].isActive = 0;
    mvaddch(enemies[i].y, enemies[i].x, ' ');
}

int activateRandomEnemy() {
    srand(time(NULL)); // Ideally, call this once at the start of your program
    for (int i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].isActive) {
            enemies[i].x = rand() % (COLS - 2) + 1;
            enemies[i].y = 1;
            enemies[i].isActive = 1;
            enqueue(i); // Add the index of the activated enemy to the queue
            return 1;
        }
    }
    return 0;
}