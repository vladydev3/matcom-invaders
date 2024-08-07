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

    // Example condition based on score (assuming you have a way to track score)
    if (score - lastScore > 50) { // If the player has gained 100 points since the last check
        activateEnemies(3); // Activate 3 enemies
        lastScore = score; // Update the last score
    }

    // You can add more conditions here based on time, levels, etc.
}
