#ifndef SHIP_H
#define SHIP_H

typedef struct {
    int x, y;
} Position;

typedef struct {
    int x, y;
    int isActive;
    struct Shot *next;
} Shot;

extern Position shipPosition;
extern Shot *shots;
extern int shotsCount;
extern volatile int gameRunning;

void* moveShip();
void addShot(int x, int y);
void moveShots();
void cleanupShots();

#endif // SHIP_H