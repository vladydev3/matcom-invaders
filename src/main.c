#include <ncurses.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "../include/ship.h"
#include "../include/enemy.h"
#include "../include/scheduling.h"

#define FPS 60
#define FRAME_DELAY (1000000 / FPS) // Microsegundos por frame

// Variable global para controlar la ejecución del hilo de disparos
volatile int keepRunningShots = 1;
volatile int gameRunning = 1;
volatile int enemyMoveTimer = 0;
int iterationCount = 0;

// Función para el hilo que maneja los disparos
void* moveShotsThread(void* arg) {
    while (keepRunningShots) {
        moveShots(); // Llama a tu función existente que mueve los disparos
        usleep(50000); // Espera un breve periodo antes de mover los disparos de nuevo
    }
    return NULL;
}

void displayLives() {
    mvprintw(0, 0, "Lives: %d", life); // Display lives at the top left corner
    refresh(); // Refresh the screen to update the display
}

void displayScore() {
    mvprintw(0, COLS - 10, "Score: %d", score); // Display score at the top right corner
    refresh(); // Refresh the screen to update the display
}

int showMenu() {
    int choice = 0;
    int highlight = 0;

    while(1) {
        clear();
        mvprintw(0, 0, "Welcome to the Game");
        mvprintw(2, 0, "1. Start Game");
        mvprintw(3, 0, "2. Exit");

        // Highlight the current selection
        if (highlight == 0) {
            mvprintw(2, 0, "> Start Game");
        } else {
            mvprintw(3, 0, "> Exit");
        }

        int key = getch();
        switch(key) {
            case KEY_UP:
            case 'w':
                highlight = (highlight - 1 + 2) % 2; // Navigate up
                break;
            case KEY_DOWN:
            case 's':
                highlight = (highlight + 1) % 2; // Navigate down
                break;
            case '\n': // Enter key
                choice = highlight + 1;
                return choice;
        }
    }
}

int main() {
    // Inicializar ncurses
    initscr(); // Inicia el modo ncurses
    cbreak(); // Deshabilita el buffer de línea, permitiendo que los caracteres sean leídos de inmediato
    noecho(); // No mostrar los caracteres leídos en la pantalla
    keypad(stdscr, TRUE); // Habilita la lectura de teclas de función, flechas, etc.
    curs_set(0); // Oculta el cursor
    // Dibuja un borde alrededor de la ventana estándar
    box(stdscr, 0, 0);

    int choice = showMenu(); // Display the menu and get the user's choice
    if (choice == 2) {
        endwin(); // End ncurses mode
        return 0; // Exit the program
    }

    clear();

    // Inicializa la posición de la nave
    shipPosition.x = COLS / 2;
    shipPosition.y = LINES - 2; // La nave aparece en la parte inferior de la pantalla

    // Crea un hilo para manejar el movimiento de la nave
    pthread_t shipThread;
    pthread_create(&shipThread, NULL, moveShip, NULL);

    // Crea un hilo para manejar el movimiento de los disparos
    pthread_t shotsThread;
    pthread_create(&shotsThread, NULL, moveShotsThread, NULL);

    // Bucle principal del juego
    while (gameRunning) {

        scheduleEnemyAppearances();

        displayLives(); // Muestra las vidas restantes

        displayScore(); // Muestra la puntuación
        
        drawEnemies(); // Dibuja los enemigos

        checkCollisions(); // Verifica si un disparo ha impactado a un enemigo


        // Incrementa el temporizador y mueve los enemigos si es el momento
        enemyMoveTimer++;
        if (enemyMoveTimer >= 20) { // Ajusta este valor para controlar la velocidad
            moveEnemiesDown(); // Mueve los enemigos hacia abajo
            enemyMoveTimer = 0; // Reinicia el temporizador
        }

        // Incrementa el contador de iteraciones
        iterationCount++;

        if (iterationCount % 100 == 0) {
            activateRandomEnemy();
        }

        refresh(); // Actualiza la pantalla

        usleep(FRAME_DELAY); // Espera un breve periodo para mantener el framerate
    }

    displayLives(); // Muestra las vidas restantes

    // limpia la pantalla
    clear();

    // Mostrar GAME OVER y puntuacion
    mvprintw(LINES / 2, COLS / 2 - 5, "GAME OVER");
    mvprintw(LINES / 2 + 1, COLS / 2 - 5, "Score: %d", score);

    // Finaliza el modo ncurses
    endwin();

    // Detiene los hilos
    keepRunningShots = 0;

    // Espera a que el hilo termine
    pthread_join(shipThread, NULL);
    pthread_join(shotsThread, NULL);

    return 0;
}