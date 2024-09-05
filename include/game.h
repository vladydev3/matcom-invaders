#ifndef GAME_H
#define GAME_H

pthread_mutex_t mutexMoveShip = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexMoveShots = PTHREAD_MUTEX_INITIALIZER;

int showMenu();
void game();

#endif