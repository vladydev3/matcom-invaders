#include <ncurses.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "../include/ship.h"
#include "../include/enemy.h"
#include "../include/scheduling.h"
#include "../include/game.h"
#include "../include/position.h"
#include "../include/score.h"



#define FPS 60
#define FRAME_DELAY (1000000 / FPS) // Microsegundos por frame

// Variable global para controlar la ejecución del hilo de disparos
volatile int keepRunningShots = 1;
volatile int keepMovingShip = 1;
volatile int gameRunning = 1;
volatile int enemyMoveTimer = 0;
pthread_mutex_t mutexMoveShip = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMoveShots = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexScreen = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el acceso a la pantalla
extern PositionEnemy* lruTail;



void* moveShipThread(void* arg) {
    while (keepMovingShip) {
        moveShip();
        usleep(50000); // Espera un breve periodo antes de mover la nave de nuevo
    }
    return NULL;
}

void* moveShotsThread(void* arg) {
    while (keepRunningShots) {        
        moveShots(); 
        usleep(50000); // Espera un breve periodo antes de mover los disparos de nuevo
    }
    return NULL;
}

void displayLives() {
     mvprintw(0, 0, "Lives: %d", life); // Display lives at the top left corner
 
}

void displayScore() {
    mvprintw(0, COLS - 10, "Score: %d", score); // Display score at the top right corner

}


int showMenu() {
    int choice = 0;
    int highlight = 0;

    while(1) {
        clear();
        refresh();
        mvprintw(0, 0, "Welcome to the Game");
        mvprintw(2, 0, "1. Start Game");
        mvprintw(3, 0, "2. Exit");

        // Highlight the current selection
        if (highlight == 0) {
            mvprintw(2, 0, ">  Start Game");
        } else {
            mvprintw(3, 0, ">  Exit");
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

void gameOver() {
    // limpia la pantalla
    clear();
    gameRunning=0;

    int high_score = read_score();

    if (score > high_score) {
        write_score(score);
        high_score = score;
    }

    // Mostrar GAME OVER y puntuacion
    mvprintw(LINES / 2, COLS / 2 - 5, "GAME OVER");
    mvprintw(LINES / 2 + 1, COLS / 2 - 5, "Score: %d", score);
    mvprintw(LINES / 2 + 2, COLS / 2 - 7, "High Score: %d", high_score);
    mvprintw(LINES / 2 + 3, COLS / 2 - 10, "Press ESC to exit...");

}

void game() {
    clear();

    // Inicializa la posición de la nave
    shipPosition.x = COLS / 2;
    shipPosition.y = LINES - 2; // La nave aparece en la parte inferior de la pantalla

    pthread_mutex_init(&mutexMoveShip, NULL);
    pthread_mutex_init(&mutexMoveShots, NULL);

    // Crea un hilo para manejar el movimiento de la nave
    pthread_t shipThread;
    pthread_create(&shipThread, NULL, moveShipThread, NULL);

    // Crea un hilo para manejar el movimiento de los disparos
    pthread_t shotsThread;
    pthread_create(&shotsThread, NULL, moveShotsThread, NULL);
    initializeLandingPositions(7);
    // Bucle principal del juego
    while (gameRunning) {
        schedule_enemies_fcfs(lruTail); 

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

        usleep(FRAME_DELAY); // Espera un breve periodo para mantener el framerate
    }

    displayLives(); // Muestra las vidas restantes

    gameOver();

    // Detiene los hilos
    keepMovingShip = 0;
    keepRunningShots = 0;

    pthread_mutex_destroy(&mutexMoveShip);
    pthread_mutex_destroy(&mutexMoveShots);

    // Espera a que el hilo termine
    pthread_join(shipThread, NULL);
    pthread_join(shotsThread, NULL);

    // Finaliza el modo ncurses
    endwin();
}

int main() {        
    srand(time(NULL));
    // Inicializar ncurses
    initscr(); // Inicia el modo ncurses
    cbreak(); // Deshabilita el buffer de línea, permitiendo que los caracteres sean leídos de inmediato
    noecho(); // No mostrar los caracteres leídos en la pantalla
    keypad(stdscr, TRUE); // Habilita la lectura de teclas de función, flechas, etc.
    curs_set(0); // Oculta el cursor

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

    int choice = showMenu(); // Display the menu and get the user's choice
    if (choice == 2) {
        endwin(); // End ncurses mode
        return 0; // Exit the program
    }

    game();
    
    return 0;
}