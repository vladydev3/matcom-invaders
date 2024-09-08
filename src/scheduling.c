#include "threads.h"
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#include "../include/enemy.h"
#include "../include/scheduling.h"
#include "../include/position.h"

void schedule_enemies_fcfs(PositionEnemy* pos) {
    // frame count y el score del jugador (variable global) para controlar la velocidad de los enemigos
    static int frameCount = 0;
    frameCount++;
    // dependiendo del score, los enemigos se activarán cada vez más rápido
    if (frameCount % (60 - score / 100) == 0) {
        activateEnemyLRU();
        activateEnemy(pos);
    }

    while (activationRequestHead != NULL) {
        EnemyActivationRequest* request = activationRequestHead;
        addEnemy(pos->x,pos->y); // Usamos la función existente para agregar el enemigo
        activationRequestHead = request->next;
        free(request);
    }
}