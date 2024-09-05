#ifndef GAME_H
#define GAME_H
#include <pthread.h>    

extern pthread_mutex_t mutexMoveShip;
extern pthread_mutex_t mutexMoveShots;

int showMenu();
void game();

#endif