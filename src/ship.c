#include "../include/ship.h"
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

Position shipPosition;

void* moveShip(void* arg) {
    int ch;
    int prevX = shipPosition.x;
    int prevY = shipPosition.y;

    // Make getch non-blocking
    nodelay(stdscr, TRUE);

    while ((ch = getch()) != 'q') {
        if (ch != ERR) { // Check if a key was pressed
            prevX = shipPosition.x;
            prevY = shipPosition.y;

            switch (ch) {
                case KEY_LEFT:
                    if (shipPosition.x > 1) shipPosition.x--;
                    break;
                case KEY_RIGHT:
                    if (shipPosition.x < COLS - 2) shipPosition.x++;
                    break;
                case ' ':
                    addShot(shipPosition.x, shipPosition.y - 1);
                    break;
                // Additional cases for up/down if desired
            }

            mvaddch(prevY, prevX, ' ');
            mvprintw(shipPosition.y, shipPosition.x, "A");
            refresh();
        }
        // Add a small delay to prevent CPU overuse
        usleep(10000); // Adjust delay as needed for game responsiveness
    }
    return NULL;
}

// Lista dinámica de disparos
Shot *shots = NULL;
int shotsCount = 0;

// Función para agregar un disparo
void addShot(int x, int y) {
    shots = realloc(shots, (shotsCount + 1) * sizeof(Shot));
    shots[shotsCount].x = x;
    shots[shotsCount].y = y;
    shots[shotsCount].isActive = 1;
    shotsCount++;
}

// Función para mover los disparos
void moveShots() {
    for (int i = 0; i < shotsCount; i++) {
        if (shots[i].isActive) {
            // Borra el disparo en la posición actual
            mvaddch(shots[i].y, shots[i].x, ' ');
            shots[i].y--; // Mueve el disparo hacia arriba

            // Verifica si el disparo salió de la pantalla
            if (shots[i].y <= 0) {
                shots[i].isActive = 0;
            } else {
                // Dibuja el disparo en la nueva posición
                mvaddch(shots[i].y, shots[i].x, '|');
            }
        }
    }
}

// Función para limpiar los disparos inactivos
void cleanupShots() {
    int activeShots = 0;
    for (int i = 0; i < shotsCount; i++) {
        if (shots[i].isActive) {
            shots[activeShots++] = shots[i];
        }
    }
    shotsCount = activeShots;
    shots = realloc(shots, shotsCount * sizeof(Shot));
}