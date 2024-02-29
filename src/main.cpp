#include "grid.h"

int main() {
    Grid grid;
    grid.initialize();

    // Run simulation for a certain number of steps
    for (int step = 0; step < 100; ++step) {
        grid.update();
    }

    // Print final state
    grid.print();

    return 0;
}
