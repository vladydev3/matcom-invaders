#include "../include/enemy.h"

// Function to activate a specific number of enemies
void activateEnemies(int count) {
    int activatedCount = 0;
    while(activatedCount < count && activateRandomEnemy()) {
        activatedCount++;
    }
}

// Scheduler function to plan enemy appearances
void scheduleEnemyAppearances() {
    static int lastScore = 0; // Keep track of the last score to determine changes

    if (score - lastScore > 20) { // If the player has gained 100 points since the last check
        activateEnemies(1); // Activate 1 enemy
        lastScore = score; // Update the last score
    }
}
