#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "../include/enemy.h"
#include "../include/position.h"
#include "../include/scheduling.h"
// Lista LRU de posiciones donde pueden aterrizar los enemigos
PositionEnemy* lruHead = NULL;  // Cabeza de la lista LRU
PositionEnemy* lruTail = NULL;  // Cola de la lista LRU (el más antiguo)
// Inicializar la lista LRU con posiciones predefinidas (representan coordenadas de la Tierra)
void initializeLandingPositions(int numPositions) {
    for (int i = 0; i < numPositions; i++) {
        PositionEnemy* newPos = (PositionEnemy*)malloc(sizeof(PositionEnemy));
        
        newPos->x = rand() % 100;  // Coordenadas aleatorias dentro del ancho de la pantalla (ejemplo: 0 a 800)
        newPos->y = 0;             // Y fijo en 0, que es la parte superior de la pantalla
        
        newPos->next = NULL;

        // Inserta la nueva posición en la lista LRU
        if (lruHead == NULL) {
            lruHead = lruTail = newPos;  // Si la lista está vacía, cabeza y cola apuntan al nuevo nodo
        } else {
            lruTail->next = newPos;  // Agregar el nuevo nodo al final de la lista
            lruTail = newPos;        // Actualizar la cola para que sea el nuevo nodo
        }
    }
}

// Función que mueve una posición a la cabeza de la lista (más reciente)
void moveToHead(PositionEnemy* pos) {
    if (pos == lruHead) return; // Ya está en la cabeza
    // Si está en medio de la lista o al final
    PositionEnemy* temp = lruHead;
    while (temp->next != pos) {
        temp = temp->next;
    }
    temp->next = pos->next; // Eliminar pos de su lugar actual
    if (pos == lruTail) {
        lruTail = temp; // Si era la cola, ahora la cola es el anterior
    }
    pos->next = lruHead;
    lruHead = pos; // Mover a la cabeza
}

// Función para activar un enemigo en una posición basada en el algoritmo LRU
void activateEnemyLRU() {
    if (lruTail == NULL) return; // No hay posiciones disponibles
    // La cola de la lista LRU es la posición que no ha sido usada en más tiempo
    PositionEnemy* pos = lruTail;
    // Ahora esa posición es la más recientemente usada, así que la movemos a la cabeza
    moveToHead(pos);
}

// Función para liberar la memoria de las posiciones
void freeLandingPositions() {
    while (lruHead != NULL) {
        PositionEnemy* temp = lruHead;
        lruHead = lruHead->next;
        free(temp);
    }
}