#include "../include/ship.h"
#include "../include/game.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>


Position shipPosition;

void* moveShip() {
    int ch;
    int prevX = shipPosition.x;
    int prevY = shipPosition.y;

    // Make getch non-blocking
    nodelay(stdscr, TRUE);

    pthread_mutex_lock(&mutexMoveShip);

    while ((ch = getch()) != 27 ) {
        prevX = shipPosition.x;
        prevY = shipPosition.y;

        switch (ch) {
            case KEY_LEFT:
            case 'a':
                if (shipPosition.x > 0) shipPosition.x--;
                break;
            case KEY_RIGHT:
            case 'd':
                if (shipPosition.x < COLS - 1) shipPosition.x++;
                break;
            case ' ':
                pthread_mutex_lock(&mutexMoveShots);
                addShot(shipPosition.x, shipPosition.y - 1);
                pthread_mutex_unlock(&mutexMoveShots);
                break;
            case 'q':
                gameOver();
            default:
                break;

        }
        mvaddch(prevY, prevX, ' '); // Clear previous position
        mvprintw(shipPosition.y, shipPosition.x, "A");

        pthread_mutex_unlock(&mutexMoveShip);
        refresh();
        // Add a small delay to prevent CPU overuse
        usleep(10000); 
    }
    gameOver();
    return NULL;
}

// Lista dinámica de disparos
Shot *shots = NULL;
int shotsCount = 0;

// Función para agregar un disparo
void addShot(int x, int y) {
    Shot *newShot = (Shot *)malloc(sizeof(Shot));
    if (newShot == NULL) {
        // error de memoria
        return;
    }
    newShot->x = x;
    newShot->y = y;
    newShot->isActive = 1;
    newShot->next = shots; // El nuevo disparo apunta al que era el primero
    shots = newShot; // Ahora el nuevo disparo es el primero de la lista
    shotsCount++;
}

// Función para mover los disparos
void moveShots() {
    pthread_mutex_lock(&mutexMoveShots);
    Shot *current = shots;
    while (current != NULL) {
        if (current->isActive) {
            mvaddch(current->y, current->x, ' '); // Borra el disparo
            current->y--; // Mueve el disparo hacia arriba
            if (current->y <= 0) {
                current->isActive = 0; // Desactiva el disparo si sale de la pantalla
            } else {
                mvaddch(current->y, current->x, '|'); // Dibuja el disparo en la nueva posición
            }
        }
        current = current->next;
    }
    pthread_mutex_unlock(&mutexMoveShots);
}

// Función para limpiar los disparos inactivos
void cleanupShots() {
    Shot *current = shots;
    Shot *prev = NULL;
    while (current != NULL) {
        if (!current->isActive) {
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                shots = current->next;
            }
            Shot *toDelete = current;
            current = current->next;
            free(toDelete);
            shotsCount--;
        } else {
            prev = current;
            current = current->next;
        }
    }
}